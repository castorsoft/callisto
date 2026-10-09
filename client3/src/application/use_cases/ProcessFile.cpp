#include "ProcessFile.h"

#include <QFileInfo>
#include "../../core/entities/Pessoa.h"
#include "../../core/entities/Endereco.h"
#include <zafra/document/document_factory.hpp>
#include <zafra/xml/xml_document.hpp>
#include "../../infrastructure/database/repositories/DocumentoFiscalRepository.h"
#include "../../infrastructure/database/repositories/DocumentoFiscalArquivoRepository.h"
#include "../../infrastructure/database/repositories/PessoaRepository.h"
#include "../../infrastructure/database/repositories/CidadeRepository.h"
#include "../../core/GlobalState.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <optional>
#include <algorithm>

#include "../../infrastructure/database/repositories/ArquivoRepository.h"

ProcessFile::ProcessFile(QObject *parent): QObject(parent)
{
    m_processPool.setMaxThreadCount(std::max(2, QThread::idealThreadCount() / 2));
    qInfo() << "[ProcessFile] Pool de processamento fiscal iniciado com"
            << m_processPool.maxThreadCount() << "threads.";
}

ProcessFile::~ProcessFile()
{
    m_processPool.clear();
    m_processPool.waitForDone(5000);
}

void ProcessFile::execute(const QUuid &uuid)
{
    m_processPool.start([this, uuid]() {
        processFileTask(uuid);
    });
}

void ProcessFile::processFileTask(const QUuid &uuid)
{
    ArquivoRepository repo;
    auto optArquivo = repo.buscarPorUuid(uuid);
    if (!optArquivo) {
        qCritical() << "[ProcessFile] Arquivo nao encontrado no banco para o uuid:" << uuid;
        return;
    }
    Arquivo modelArquivo = optArquivo.value();

    emit processingStarted(modelArquivo);

    // QtConcurrent::run([this, filePath]() {
        QString filePath = modelArquivo.caminhoArquivo();
        try {
            qInfo() << "[ProcessFile] Processando arquivo" << filePath << " modelArquivo path:" << modelArquivo.caminhoArquivo();
            QFileInfo arquivo = QFileInfo(filePath);
            // Arquivo modelArquivo;

            // if (ArquivoRepository::existeArquivo(filePath, &modelArquivo)) {
            //     qInfo() << "[ProcessFile] Arquivo ja processado anteriormente:" << filePath;
            //     return;
            // }

            // auto modelArquivo = Arquivo(arquivo.absoluteFilePath(), arquivo.fileName(), false)
            // ArquivoRepository::salvar(&modelArquivo);

            zafra::xml::XmlDocument xmlReader;
            xmlReader.LoadFromFile(filePath.toStdString());
            qInfo() << "Nome primeiro node:" << xmlReader.GetRoot()->GetName();
            auto doc = zafra::document::DocumentFactory::DetectAndCreate(xmlReader.GetRoot().get());
            if (!doc) {
                qCritical() << "[ProcessFile] Falha ao decodificar arquivo, nulo retornado:" << filePath;
                emit fileError(modelArquivo, "Estrutura XML não reconhecida.");
                return;
            }

            if (doc->GetType() == zafra::core::DocumentType::Unknown) {
            // auto modelo = zafra::identificarModelo(filePath.toStdString());
            // if (doc == zafra::NAO_IDENTIFICADO) {
                // Modelo não catalogado.
                qInfo() << "[ProcessFile] Documento possui uma estrutura nao suportada:" << filePath;
                return;
            }

            switch (doc->GetType())
            {
                case zafra::core::DocumentType::NFe:
                    processarNFe(static_cast<const zafra::document::NFeDocument&>(*doc), filePath);
                    break;
                default:
                    break;
            }

            // auto documentoFiscal = zafra::carregarDocumento(filepath.toStdString(), modelo);

            // auto modelDocumentoFiscalArquivo = DocumentoFiscalArquivo(documentoFiscal.xml(), documentoFiscal.chaveAcesso(), jsonCabulosoAqui, modelo);

            // Primeiro verifica se é um arquivo suportado (.xml ou .json) - Já vem filtrado do Watcher
            // Registramos o arquivo na tabela arquivos, para indicar que já pegamos o arquivo para processar
            // Depois usamos a lib pra identificar qual o modelo do documento (Se é nfe, nfce, nfse [padrão nacional, abrasf, ou IPM inicialmente])
            // Carregamos o documento, registramos ele na documentos_fiscais_arquivo com os dados necessários (chave e modelo do xml);
            // Registramos os demais dados nas tabelas documentos_fiscais e documentos_fiscais_itens.
            // Caso a nota seja de um terceiro e haja produtos, devemos preencher a tabela dos produtos do fornecedor para posterior associação

            emit processingFinished(modelArquivo, true);

        } catch(const std::exception &e) {
            emit fileError(modelArquivo, QString::fromLocal8Bit(e.what()));
        }
    // });
}

void ProcessFile::processarNFe(const zafra::document::NFeDocument &doc, const QString &filePath)
{
    qInfo() << "[ProcessarNFe] << Processando NFe" << QString::fromStdString(doc.getChaveAcesso());

    DocumentoFiscalRepository repo;
    PessoaRepository pesRepo;

    if (repo.existeDocumento(QString::fromStdString(doc.getChaveAcesso()))) {
        qInfo() << "[ProcessarNFe] Documento ja existe no banco de dados.";
        return;
    }

    QString chaveAcesso = QString::fromStdString(doc.getChaveAcesso());
    QString tipoOperacao = (doc.getTipoOperacao() == zafra::core::TipoOperacao::Saida) ? "saida" : "entrada";

    QString tipoEmissao = "terceiro";
    if (GlobalState::instance().empresaAtual().has_value() && doc.GetEmit()) {
        QString cnpjEmitente = QString::fromStdString(doc.GetEmit()->GetCnpjCpf());
        QString cnpjEmpresa = GlobalState::instance().empresaAtual().value().cnpjCpf();
        cnpjEmitente.remove('.').remove('-').remove('/');
        cnpjEmpresa.remove('.').remove('-').remove('/');
        if (!cnpjEmitente.isEmpty() && cnpjEmitente == cnpjEmpresa) {
            tipoEmissao = "propria";
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
    // Opcionalmente podemos salvar o XML inteiro
    // dadosDoc["xml"] = QString::fromStdString(rootNode->GetValue());

    DocumentoFiscal documento(chaveAcesso, tipoOperacao, tipoEmissao, idPessoa, modelo, serie, numero, dhEmissao, dadosDoc);

    auto dets = doc.GetDet();
    for (const auto& det: dets) {
        if (!det) continue;

        int nItem = det->GetNItem();
        auto prod = det->GetProd();

        QString cProd = QString::fromStdString(prod->GetCProd());
        QString xProd = QString::fromStdString(prod->GetXProd());
        QString ncm = QString::fromStdString(prod->GetNCM());
        QString cfop = QString::fromStdString(prod->GetCFOP());
        QString uCom = QString::fromStdString(prod->GetUCom());
        double qCom = prod->GetQCom();
        double vUnCom = prod->GetVUnCom();
        double vProd = prod->GetVProd();
        double vDesc = prod->GetVDesc().value_or(0);

        QJsonObject dadosItem;

        if (prod->GetNCM() != "") {
            dadosItem["ncm"] = QString::fromStdString(prod->GetNCM());
        }

        if (prod->GetCEAN() != "") {
            dadosItem["cest"] = QString::fromStdString(prod->GetCEST());
        }

        DocumentoFiscalItem item(-1, nItem, cProd, xProd, ncm, cfop, uCom, qCom, vUnCom, vProd, vDesc, dadosItem);

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

    if (repo.salvar(documento)) {
        qInfo() << "[ProcessarNFe] NFe salva com sucesso. ID:" << documento.id().value_or(0);

        if (documento.id().has_value() && !filePath.isEmpty()) {
            QFile fileXml(filePath);
            if (fileXml.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QString conteudoXml = QString::fromUtf8(fileXml.readAll());
                fileXml.close();

                DocumentoFiscalArquivo docArquivo(documento.id().value(), conteudoXml, "DOCUMENTO_FISCAL");
                DocumentoFiscalArquivoRepository arquivoRepo;
                if (arquivoRepo.salvar(docArquivo)) {
                    qInfo() << "[ProcessarNFe] Arquivo XML salvo com sucesso no banco. ID:" << docArquivo.id().value_or(0);
                } else {
                    qWarning() << "[ProcessarNFe] Falha ao salvar arquivo XML no banco para documento ID:" << documento.id().value();
                }
            } else {
                qWarning() << "[ProcessarNFe] Nao foi possivel abrir o arquivo para leitura do XML:" << filePath;
            }
        }
    } else {
        qCritical() << "[ProcessarNFe] Falha ao salvar NFe.";
    }
}

int ProcessFile::carregarOuCriarPessoa(const zafra::document::NFeDocument &doc, const QString &tipoEmissao) {
    static QMutex s_pessoaMutex;
    QMutexLocker locker(&s_pessoaMutex);

    PessoaRepository pessoaRepo;

    // 1. Identifica se a contraparte é o Emitente (nota de terceiro) ou Destinatário (emissão própria)
    QString cnpjCpf;
    QString razaoSocial;
    std::optional<QString> nomeFantasia;
    std::optional<QString> inscricaoEstadual;
    const zafra::document::NFeDocumentEndereco* enderXml = nullptr;
    QString isFornecedor = "N";

    if (tipoEmissao == "terceiro" && doc.GetEmit()) {
        auto emitente = doc.GetEmit();
        cnpjCpf = QString::fromStdString(emitente->GetCnpjCpf());
        razaoSocial = QString::fromStdString(emitente->GetRazaoSocial());
        if (!emitente->GetNomeFantasia().empty()) {
            nomeFantasia = QString::fromStdString(emitente->GetNomeFantasia());
        }
        if (!emitente->GetInscricaoEstadual().empty()) {
            inscricaoEstadual = QString::fromStdString(emitente->GetInscricaoEstadual());
        }
        enderXml = emitente->GetEnderEmit();
        isFornecedor = "S";
    } else if (doc.GetDest()) {
        auto dest = doc.GetDest();
        cnpjCpf = QString::fromStdString(dest->GetCnpjCpf());
        razaoSocial = QString::fromStdString(dest->GetNome());
        if (dest->GetInscricaoEstadual().has_value()) {
            inscricaoEstadual = QString::fromStdString(dest->GetInscricaoEstadual().value());
        }
        enderXml = dest->GetEnderDest();
    }

    // 2. Verifica se a Pessoa já existe cadastrada no banco
    if (!cnpjCpf.isEmpty()) {
        auto optExistente = pessoaRepo.buscarPorCnpjCpf(cnpjCpf);
        if (optExistente.has_value() && optExistente.value().id().has_value()) {
            return optExistente.value().id().value();
        }
    }

    // 3. Caso não exista, busca a Cidade pelo código IBGE do XML
    int idCidade = 1; // Fallback
    if (enderXml && enderXml->GetCodMunicipio() > 0) {
        CidadeRepository cidadeRepo;
        auto optCidade = cidadeRepo.buscarPorIbge(enderXml->GetCodMunicipio());
        if (optCidade.has_value() && optCidade.value().id() > 0) {
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
    return novaPessoa.id().value_or(1);
}
