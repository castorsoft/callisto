#pragma once

#include <zafra/document/documents/nfe_document.hpp>

#include <QString>
#include <QMetaType>

class IDocumentoFiscalRepository;
class IDocumentoFiscalArquivoRepository;
class IPessoaRepository;
class IProdutoRepository;
class IProdutoFornecedorRepository;
class IUnidadeMedidaRepository;
class INCMRepository;
class ICidadeRepository;

/**
 * @brief Dependências de repositórios injetáveis para o caso de uso ProcessFile.
 * Quando um ponteiro é nullptr, o repositório concreto padrão é utilizado.
 */
struct ProcessFileRepositories {
    IDocumentoFiscalRepository *documentoFiscalRepo = nullptr;
    IDocumentoFiscalArquivoRepository *documentoFiscalArquivoRepo = nullptr;
    IPessoaRepository *pessoaRepo = nullptr;
    IProdutoRepository *produtoRepo = nullptr;
    IProdutoFornecedorRepository *produtoFornecedorRepo = nullptr;
    IUnidadeMedidaRepository *unidadeMedidaRepo = nullptr;
    INCMRepository *ncmRepo = nullptr;
    ICidadeRepository *cidadeRepo = nullptr;
};

/**
 * @brief Resultado do processamento de um único arquivo XML.
 */
struct ResultadoProcessamento {
    enum class Status { Sucesso, JaImportado, Erro };

    Status status = Status::Erro;
    QString chaveAcesso;
    QString modelo;
    QString mensagem;
    int idDocumentoFiscal = -1;
};

Q_DECLARE_METATYPE(ResultadoProcessamento)

class ProcessFile {
public:
    ProcessFile() = default;
    explicit ProcessFile(const ProcessFileRepositories &repos);

    ResultadoProcessamento processarArquivo(const QString &filePath);
    ResultadoProcessamento processarXml(const QString &conteudoXml);

private:
    ResultadoProcessamento processarNFe(const zafra::document::NFeDocument &doc, const QString &filePath = QString());
    int carregarOuCriarPessoa(const zafra::document::NFeDocument &doc, const QString &tipoEmissao);
    int carregarOuCriarProduto(const zafra::document::NFeDocumentDet &det);
    int carregarOuCriarUnidadeMedida(const QString &sigla);
    int carregarOuCriarNCM(const QString &ncm);

    ProcessFileRepositories m_repos;
};