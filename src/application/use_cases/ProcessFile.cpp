#include "ProcessFile.h"

#include <QFileInfo>
#include "../../core/entities/Pessoa.h"
#include "../../core/entities/Endereco.h"
#include <zafra/document/document_factory.hpp>
#include <zafra/xml/xml_document.hpp>
#include "../../core/interfaces/IDocumentoFiscalRepository.h"
#include "../../core/interfaces/IDocumentoFiscalArquivoRepository.h"
#include "../../core/interfaces/IPessoaRepository.h"
#include "../../core/interfaces/IProdutoRepository.h"
#include "../../core/interfaces/IProdutoFornecedorRepository.h"
#include "../../core/interfaces/IUnidadeMedidaRepository.h"
#include "../../core/interfaces/INCMRepository.h"
#include "../../core/interfaces/ICidadeRepository.h"
#include "../../core/RepositoryProvider.h"
#include "../../core/GlobalState.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QSet>
#include <optional>

namespace {

/**
 * Impede que dois arquivos com a mesma chave de acesso sejam gravados
 * simultaneamente por threads diferentes do pool (a tabela documentos_fiscais
 * não possui restrição de unicidade na chave_acesso).
 */
class ChaveEmProcessamentoGuard {
public:
    explicit ChaveEmProcessamentoGuard(const QString &chave): m_chave(chave)
    {
        QMutexLocker locker(&s_mutex);
        if (s_chaves.contains(m_chave)) {
            m_adquirida = false;
        } else {
            s_chaves.insert(m_chave);
            m_adquirida = true;
        }
    }

    ~ChaveEmProcessamentoGuard()
    {
        if (!m_adquirida) return;
        QMutexLocker locker(&s_mutex);
        s_chaves.remove(m_chave);
    }

    bool adquirida() const { return m_adquirida; }

private:
    QString m_chave;
    bool m_adquirida = false;

    static inline QMutex s_mutex;
    static inline QSet<QString> s_chaves;
};

} // namespace

ProcessFile::ProcessFile()
{
    if (RepositoryProvider::isInitialized()) {
        auto &factory = RepositoryProvider::instance();
        m_repos.documentoFiscalRepo = &factory.documentoFiscalRepository();
        m_repos.documentoFiscalArquivoRepo = &factory.documentoFiscalArquivoRepository();
        m_repos.pessoaRepo = &factory.pessoaRepository();
        m_repos.produtoRepo = &factory.produtoRepository();
        m_repos.produtoFornecedorRepo = &factory.produtoFornecedorRepository();
        m_repos.unidadeMedidaRepo = &factory.unidadeMedidaRepository();
        m_repos.ncmRepo = &factory.ncmRepository();
        m_repos.cidadeRepo = &factory.cidadeRepository();
    }
}

ProcessFile::ProcessFile(const ProcessFileRepositories &repos)
    : m_repos(repos)
{
}

ResultadoProcessamento ProcessFile::processarArquivo(const QString &filePath)
{
    ResultadoProcessamento resultado;

    try {
        qInfo() << "[ProcessFile] Processando arquivo" << filePath;

        if (!QFileInfo::exists(filePath)) {
            resultado.mensagem = "Arquivo não encontrado.";
            return resultado;
        }

        QFile fileXml(filePath);
        if (!fileXml.open(QIODevice::ReadOnly | QIODevice::Text)) {
            resultado.mensagem = "Não foi possível abrir o arquivo para leitura.";
            return resultado;
        }
        QString conteudoXml = QString::fromUtf8(fileXml.readAll());
        fileXml.close();

        return processarXml(conteudoXml);
    } catch (const std::exception &e) {
        qCritical() << "[ProcessFile] Erro ao processar" << filePath << ":" << e.what();
        resultado.status = ResultadoProcessamento::Status::Erro;
        resultado.mensagem = QString::fromLocal8Bit(e.what());
        return resultado;
    }
}

ResultadoProcessamento ProcessFile::processarXml(const QString &conteudoXml)
{
    ResultadoProcessamento resultado;

    try {
        if (conteudoXml.trimmed().isEmpty()) {
            resultado.mensagem = "Conteúdo XML vazio.";
            return resultado;
        }

        auto doc = zafra::document::DocumentFactory::DetectAndCreate(conteudoXml.toStdString());
        if (!doc) {
            qCritical() << "[ProcessFile] Falha ao decodificar XML, nulo retornado.";
            resultado.mensagem = "Estrutura XML não reconhecida.";
            return resultado;
        }

        switch (doc->GetType())
        {
            case zafra::core::DocumentType::NFe:
                return processarNFe(static_cast<const zafra::document::NFeDocument&>(*doc));
            default:
                qInfo() << "[ProcessFile] Documento possui uma estrutura nao suportada.";
                resultado.mensagem = "Tipo de documento não suportado.";
                return resultado;
        }
    } catch (const std::exception &e) {
        qCritical() << "[ProcessFile] Erro ao processar XML:" << e.what();
        resultado.status = ResultadoProcessamento::Status::Erro;
        resultado.mensagem = QString::fromLocal8Bit(e.what());
        return resultado;
    }
}

ResultadoProcessamento ProcessFile::processarNFe(const zafra::document::NFeDocument &doc, const QString &filePath)
{
    ResultadoProcessamento resultado;

    qInfo() << "[ProcessarNFe] << Processando NFe" << QString::fromStdString(doc.getChaveAcesso());

    IDocumentoFiscalRepository &repo = m_repos.documentoFiscalRepo
        ? *m_repos.documentoFiscalRepo
        : RepositoryProvider::documentoFiscalRepository();

    QString chaveAcesso = QString::fromStdString(doc.getChaveAcesso());
    resultado.chaveAcesso = chaveAcesso;
    resultado.modelo = doc.GetIde() ? QString::fromStdString(doc.GetIde()->GetMod()) : "55";

    ChaveEmProcessamentoGuard guard(chaveAcesso);
    if (!guard.adquirida() || repo.existeDocumento(chaveAcesso)) {
        qInfo() << "[ProcessarNFe] Documento ja existe no banco de dados.";
        resultado.status = ResultadoProcessamento::Status::JaImportado;
        resultado.mensagem = "Documento já importado anteriormente.";
        return resultado;
    }

    // Obtém o conteúdo do XML diretamente do documento (SetXML/getXML)
    QString conteudoXml = QString::fromStdString(doc.getXML());
    if (conteudoXml.isEmpty() && !filePath.isEmpty()) {
        QFile fileXml(filePath);
        if (fileXml.open(QIODevice::ReadOnly | QIODevice::Text)) {
            conteudoXml = QString::fromUtf8(fileXml.readAll());
            fileXml.close();
        }
    }

    if (conteudoXml.isEmpty()) {
        resultado.mensagem = "Conteúdo XML do documento está vazio.";
        return resultado;
    }

    QString tipoOperacao = (doc.getTipoOperacao() == zafra::core::TipoOperacao::Saida) ? "saida" : "entrada";

    QString tipoEmissao = TipoEmissao::Terceiro;
    if (GlobalState::instance().empresaAtual().has_value() && doc.GetEmit()) {
        QString cnpjEmitente = QString::fromStdString(doc.GetEmit()->GetCnpjCpf());
        QString cnpjEmpresa = GlobalState::instance().empresaAtual().value().cnpjCpf();
        cnpjEmitente.remove('.').remove('-').remove('/');
        cnpjEmpresa.remove('.').remove('-').remove('/');
        if (!cnpjEmitente.isEmpty() && cnpjEmitente == cnpjEmpresa) {
            tipoEmissao = TipoEmissao::Propria;
        }
    }

    int idPessoa = carregarOuCriarPessoa(doc, tipoEmissao);

    QString modelo = "55";
    QString serie = "0";
    int numero = 0;
    QDateTime dhEmissao = QDateTime::currentDateTime();

    if (doc.GetIde()) {
        modelo = QString::fromStdString(doc.GetIde()->GetMod());
        serie = QString::number(doc.GetIde()->GetSerie());
        numero = doc.GetIde()->GetNNf();
        dhEmissao = QDateTime::fromSecsSinceEpoch(doc.GetIde()->GetDhEmissao());
    }

    QJsonObject dadosDoc;

    DocumentoFiscal documento(chaveAcesso, tipoOperacao, tipoEmissao, idPessoa, modelo, serie, numero, dhEmissao, dadosDoc);

    auto dets = doc.GetDet();
    for (const auto& det: dets) {
        if (!det) continue;

        if (tipoEmissao == TipoEmissao::Propria) {
            carregarOuCriarProduto(*det);
        }

        int nItem = det->GetNItem();

        auto prod = det->GetProd();

        QString cProd = QString::fromStdString(prod->GetCProd());
        QString xProd = QString::fromStdString(prod->GetXProd());
        QString uCom = QString::fromStdString(prod->GetUCom());
        double qCom = prod->GetQCom();
        double vUnCom = prod->GetVUnCom();
        double vProd = prod->GetVProd();
        double vDesc = prod->GetVDesc().value_or(0);

        QJsonObject dadosItem;

        if (prod->GetNCM() != "") {
            dadosItem["ncm"] = QString::fromStdString(prod->GetNCM());
        }

        if (prod->GetCFOP() != "") {
            dadosItem["cfop"] = QString::fromStdString(prod->GetCFOP());
        }

        QString cEAN = QString::fromStdString(prod->GetCEAN()).trimmed();
        if (cEAN.compare("SEM GTIN", Qt::CaseInsensitive) == 0) {
            cEAN.clear();
        }

        if (prod->GetCEST() != "") {
            dadosItem["cest"] = QString::fromStdString(prod->GetCEST());
        }

        if (!cEAN.isEmpty()) {
            dadosItem["cEAN"] = cEAN;
        }

        DocumentoFiscalItem item(-1, nItem, cProd, xProd, uCom, qCom, vUnCom, vProd, vDesc, dadosItem, cEAN);

        if (tipoEmissao == TipoEmissao::Terceiro) {
            IProdutoFornecedorRepository &pfRepo = m_repos.produtoFornecedorRepo
                ? *m_repos.produtoFornecedorRepo
                : RepositoryProvider::produtoFornecedorRepository();

            auto optPf = pfRepo.buscarPorFornecedorECodigo(idPessoa, cProd);
            if (!optPf.has_value() && !cEAN.isEmpty()) {
                IProdutoRepository &prodRepo = m_repos.produtoRepo
                    ? *m_repos.produtoRepo
                    : RepositoryProvider::produtoRepository();

                auto optProd = prodRepo.buscarPorCodigoBarra(cEAN);
                if (optProd.has_value()) {
                    Produto prodExistente = optProd.value();
                    ProdutoFornecedor pf(
                        prodExistente.id().value_or(0),
                        idPessoa,
                        cProd,
                        prodExistente.idUnidadeMedida(),
                        1.0,
                        std::nullopt
                    );
                    if (pfRepo.salvar(pf)) {
                        qInfo() << "[ProcessFile] Auto-associacao criada com sucesso via codigo de barras para produto:"
                                << prodExistente.descricao() << "e fornecedor:" << idPessoa;
                    }
                }
            }
        }

        auto imposto = det->GetImposto();
        if (imposto) {
            if (imposto->GetICMS().has_value()) {
                QJsonObject icmsObj;
                auto icms = imposto->GetICMS().value();
                if (icms) {
                    if (icms->GetCST().has_value()) {
                        icmsObj["cst"] = QString::fromStdString(icms->GetCST().value());
                    } else if (icms->GetCSOSN().has_value()) {
                        icmsObj["csosn"] = QString::fromStdString(icms->GetCSOSN().value());
                    }

                    icmsObj["origem"] = QString::fromStdString(icms->GetOrig());

                    if (icms->GetVBC().has_value()) {
                        icmsObj["base_calculo"] = QString::number(icms->GetVBC().value(), 'f', 4);
                    }
                    if (icms->GetPICMS().has_value()) {
                        icmsObj["aliquota"] = QString::number(icms->GetPICMS().value(), 'f', 4);
                    }
                    if (icms->GetVICMS().has_value()) {
                        icmsObj["valor"] = QString::number(icms->GetVICMS().value(), 'f', 4);
                    }

                    DocumentoFiscalItemTributo tributoIcms(-1, "icms", icmsObj);

                    item.adicionarTributo(tributoIcms);
                }
            }

            if (imposto->GetPIS().has_value()) {
                QJsonObject pisObj;
                auto pis = imposto->GetPIS().value();
                if (pis) {
                    pisObj["cst"] = QString::fromStdString(pis->GetCST());

                    if (pis->GetVBC().has_value()) {
                        pisObj["base_calculo"] = QString::number(pis->GetVBC().value(), 'f', 4);
                    }

                    if (pis->GetPPIS().has_value()) {
                        pisObj["aliquota"] = QString::number(pis->GetPPIS().value(), 'f', 4);
                    }

                    pisObj["valor"] = QString::number(pis->GetVPIS(), 'f', 4);
                }

                DocumentoFiscalItemTributo tributoPis(-1, "pis", pisObj);

                item.adicionarTributo(tributoPis);
            }

            if (imposto->GetCOFINS().has_value()) {
                QJsonObject cofinsObj;
                auto cofins = imposto->GetCOFINS().value();
                if (cofins) {
                    cofinsObj["cst"] = QString::fromStdString(cofins->GetCST());

                    if (cofins->GetVBC().has_value()) {
                        cofinsObj["base_calculo"] = QString::number(cofins->GetVBC().value(), 'f', 4);
                    }

                    if (cofins->GetPCOFINS().has_value()) {
                        cofinsObj["aliquota"] = QString::number(cofins->GetPCOFINS().value(), 'f', 4);
                    }

                    cofinsObj["valor"] = QString::number(cofins->GetVCOFINS(), 'f', 4);
                }

                DocumentoFiscalItemTributo tributoCofins(-1, "cofins", cofinsObj);

                item.adicionarTributo(tributoCofins);
            }
            if (imposto->GetIPI().has_value()) {
                QJsonObject ipiObj;
                auto ipi = imposto->GetIPI().value();
                if (ipi) {
                    ipiObj["cst"] = QString::fromStdString(ipi->GetCST());

                    if (ipi->GetVBC().has_value()) {
                        ipiObj["base_calculo"] = QString::number(ipi->GetVBC().value(), 'f', 4);
                    }

                    if (ipi->GetPIPI().has_value()) {
                        ipiObj["aliquota"] = QString::number(ipi->GetPIPI().value(), 'f', 4);
                    }

                    if (ipi->GetVIPI().has_value()) {
                        ipiObj["valor"] = QString::number(ipi->GetVIPI().value(), 'f', 4);
                    }
                }

                DocumentoFiscalItemTributo tributoIpi(-1, "ipi", ipiObj);

                item.adicionarTributo(tributoIpi);
            }
        }

        documento.adicionarItem(item);
    }

    if (!repo.salvar(documento) || !documento.id().has_value()) {
        qCritical() << "[ProcessarNFe] Falha ao salvar NFe.";
        resultado.mensagem = "Falha ao gravar o documento no banco de dados.";
        return resultado;
    }

    qInfo() << "[ProcessarNFe] NFe salva com sucesso. ID:" << documento.id().value();
    resultado.idDocumentoFiscal = documento.id().value();
    resultado.status = ResultadoProcessamento::Status::Sucesso;
    resultado.mensagem = QString("Documento ID %1").arg(resultado.idDocumentoFiscal);

    DocumentoFiscalArquivo docArquivo(documento.id().value(), conteudoXml, "DOCUMENTO_FISCAL");
    IDocumentoFiscalArquivoRepository &arquivoRepo = m_repos.documentoFiscalArquivoRepo
        ? *m_repos.documentoFiscalArquivoRepo
        : RepositoryProvider::documentoFiscalArquivoRepository();

    if (arquivoRepo.salvar(docArquivo)) {
        qInfo() << "[ProcessarNFe] Arquivo XML salvo com sucesso no banco. ID:" << docArquivo.id().value_or(0);
    } else {
        qWarning() << "[ProcessarNFe] Falha ao salvar arquivo XML no banco para documento ID:" << documento.id().value();
        resultado.mensagem += " (aviso: XML não foi armazenado)";
    }

    return resultado;
}

int ProcessFile::carregarOuCriarPessoa(const zafra::document::NFeDocument &doc, const QString &tipoEmissao) {
    static QMutex s_pessoaMutex;
    QMutexLocker locker(&s_pessoaMutex);

    IPessoaRepository &pessoaRepo = m_repos.pessoaRepo
        ? *m_repos.pessoaRepo
        : RepositoryProvider::pessoaRepository();

    QString cnpjCpf;
    QString razaoSocial;
    std::optional<QString> nomeFantasia;
    std::optional<QString> inscricaoEstadual;
    const zafra::document::NFeDocumentEndereco* enderXml = nullptr;
    QString isFornecedor = "N";

    // 1. Identifica se a outra pessoa é o emitente ou o destinatário da nota
    if (tipoEmissao == "terceiro") {
        auto emitente = doc.GetEmit();
        if (emitente) {
            cnpjCpf = QString::fromStdString(emitente->GetCnpjCpf());
            razaoSocial = QString::fromStdString(emitente->GetRazaoSocial());
            if (!emitente->GetNomeFantasia().empty()) {
                nomeFantasia = QString::fromStdString(emitente->GetNomeFantasia());
            }
            if (!emitente->GetInscricaoEstadual().empty()) {
                inscricaoEstadual = QString::fromStdString(emitente->GetInscricaoEstadual());
            }
            enderXml = emitente->GetEnderEmit();
        }
        isFornecedor = "S";
    } else {
        auto dest = doc.GetDest();
        if (dest) {
            // TODO: estrangeiro
            cnpjCpf = QString::fromStdString(dest->GetCnpjCpf().empty() ? "00000000000" : dest->GetCnpjCpf());
            razaoSocial = QString::fromStdString(dest->GetNome());
            if (dest->GetInscricaoEstadual().has_value()) {
                inscricaoEstadual = QString::fromStdString(dest->GetInscricaoEstadual().value());
            }
            enderXml = dest->GetEnderDest();
        }
    }

    // 2. Verifica se a Pessoa já existe cadastrada no banco
    if (!cnpjCpf.isEmpty()) {
        auto optExistente = pessoaRepo.buscarPorCnpjCpf(cnpjCpf);
        if (optExistente.has_value()) {
            return optExistente.value().id().value_or(0);
        }
    }

    // 3. Caso não exista, busca a Cidade pelo código IBGE do XML
    int idCidade = 1; // Fallback
    if (enderXml && enderXml->GetCodMunicipio() > 0) {
        ICidadeRepository &cidadeRepo = m_repos.cidadeRepo
            ? *m_repos.cidadeRepo
            : RepositoryProvider::cidadeRepository();

        auto optCidade = cidadeRepo.buscarPorIbge(enderXml->GetCodMunicipio());
        if (optCidade.has_value()) {
            idCidade = optCidade.value().id();
        }
    }

    // 4. Cria a instância de Endereço
    Endereco endereco(
        enderXml ? QString::fromStdString(enderXml->GetLogradouro()) : "",
        enderXml ? std::make_optional(QString::fromStdString(enderXml->GetNumero())) : std::nullopt,
        (enderXml && enderXml->GetComplemento().has_value()) ? std::make_optional(QString::fromStdString(enderXml->GetComplemento().value())) : std::nullopt,
        enderXml ? QString::fromStdString(enderXml->GetBairro()) : "",
        enderXml ? QString::fromStdString(enderXml->GetCEP()) : "",
        idCidade
    );

    // 5. Obtém a empresa logada no sistema
    int idEmpresa = GlobalState::instance().empresaAtual().has_value()
        ? GlobalState::instance().empresaAtual().value().id().value_or(1)
        : 1;

    // 6. Instancia a nova Pessoa
    Pessoa novaPessoa(
        cnpjCpf,
        razaoSocial.isEmpty() ? "SEM RAZAO SOCIAL" : razaoSocial,
        nomeFantasia,
        inscricaoEstadual,
        isFornecedor,   // "S" se for emitente de nota de terceiro
        "N",            // Contabilista = "N"
        endereco,
        idEmpresa
    );

    // 7. Salva no banco (o repositório salva o endereço e preenche o id da pessoa)
    pessoaRepo.salvar(novaPessoa);

    qInfo() << "[ProcessFile] Nova Pessoa cadastrada automaticamente. ID:" << novaPessoa.id().value_or(0);
    return novaPessoa.id().value_or(0);
}

int ProcessFile::carregarOuCriarProduto(const zafra::document::NFeDocumentDet &det) {
    static QMutex s_produtoMutex;
    QMutexLocker locker(&s_produtoMutex);

    IProdutoRepository &prodRepo = m_repos.produtoRepo
        ? *m_repos.produtoRepo
        : RepositoryProvider::produtoRepository();

    std::optional<Produto> optProduto = prodRepo.buscarPorCodigoInterno(QString::fromStdString(det.GetProd()->GetCProd()));
    if (optProduto.has_value()) {
        return optProduto.value().id().value_or(0);
    }

    auto prod = det.GetProd();

    int idUnidadeMedida = carregarOuCriarUnidadeMedida(QString::fromStdString(prod->GetUCom()));
    int idNCM = carregarOuCriarNCM(QString::fromStdString(prod->GetNCM()));

    QString cEAN = QString::fromStdString(prod->GetCEAN()).trimmed();
    if (cEAN.compare("SEM GTIN", Qt::CaseInsensitive) == 0) {
        cEAN.clear();
    }

    Produto novoProduto(
        QString::fromStdString(prod->GetXProd()),
        idUnidadeMedida,
        idNCM,
        TipoProdutoAtividade::MercadoriaRevenda,
        cEAN,
        QString::fromStdString(prod->GetCProd())
    );
    prodRepo.salvar(novoProduto);

    return novoProduto.id().value_or(0);
}

int ProcessFile::carregarOuCriarUnidadeMedida(const QString &sigla) {
    static QMutex s_unidadeMutex;
    QMutexLocker locker(&s_unidadeMutex);

    IUnidadeMedidaRepository &unMedidaRepo = m_repos.unidadeMedidaRepo
        ? *m_repos.unidadeMedidaRepo
        : RepositoryProvider::unidadeMedidaRepository();

    auto optUnMedida = unMedidaRepo.buscarPorSigla(sigla);
    if (optUnMedida.has_value()) {
        return optUnMedida.value().id().value_or(0);
    }

    UnidadeMedida novaUnMedida(sigla, sigla);
    unMedidaRepo.salvar(novaUnMedida);

    qInfo() << "[ProcessFile] Nova Unidade de Medida cadastrada automaticamente. ID:" << novaUnMedida.id().value_or(0);
    return novaUnMedida.id().value_or(0);
}

int ProcessFile::carregarOuCriarNCM(const QString &codigo)
{
    static QMutex s_ncmMutex;
    QMutexLocker locker(&s_ncmMutex);

    INCMRepository &ncmRepo = m_repos.ncmRepo
        ? *m_repos.ncmRepo
        : RepositoryProvider::ncmRepository();

    auto optNCM = ncmRepo.buscarPorCodigo(codigo);
    if (optNCM.has_value()) {
        return optNCM.value().id().value_or(0);
    }

    NCM novoNCM(codigo, "CADASTRO AUTOMATICO", 0, 0, 0, QDate::currentDate(), QDate::currentDate().addYears(1));
    ncmRepo.salvar(novoNCM);

    qInfo() << "[ProcessFile] Novo NCM cadastrado automaticamente. ID:" << novoNCM.id().value_or(0);

    return novoNCM.id().value_or(0);
}
