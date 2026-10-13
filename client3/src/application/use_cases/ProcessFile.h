#pragma once

#include <zafra/document/documents/nfe_document.hpp>

#include <QString>
#include <QMetaType>

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

/**
 * @brief Caso de uso responsável por ler um arquivo XML fiscal e persisti-lo no banco.
 *
 * A execução é síncrona: quem chama decide em qual thread ela ocorre.
 * O arquivo físico não é movido nem alterado.
 */
class ProcessFile {
public:
    ProcessFile() = default;

    ResultadoProcessamento processarArquivo(const QString &filePath);
    ResultadoProcessamento processarXml(const QString &conteudoXml);

private:
    ResultadoProcessamento processarNFe(const zafra::document::NFeDocument &doc, const QString &filePath = QString());
    int carregarOuCriarPessoa(const zafra::document::NFeDocument &doc, const QString &tipoEmissao);
    int carregarOuCriarProduto(const zafra::document::NFeDocumentDet &det);
    int carregarOuCriarUnidadeMedida(const QString &sigla);
    int carregarOuCriarNCM(const QString &ncm);
};