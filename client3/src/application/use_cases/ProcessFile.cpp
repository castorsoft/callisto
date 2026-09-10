#include "ProcessFile.h"

#include <QFileInfo>
#include "../../core/entities/Arquivo.h"
#include <zafra/document/document_factory.hpp>
#include <zafra/xml/xml_document.hpp>

ProcessFile::ProcessFile(QObject *parent): QObject(parent) { }

void ProcessFile::execute(Arquivo &modelArquivo)
{
    emit processingStarted(modelArquivo);

    // QtConcurrent::run([this, filePath]() {
        QString filePath = modelArquivo.caminhoArquivo();
        try {
            qInfo() << "[ProcessFile] Processando arquivo" << filePath;
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
            auto doc = zafra::document::DocumentFactory::DetectAndCreate(xmlReader.GetRoot().get());
            if (doc->GetType() == zafra::core::DocumentType::Unknown) {
            // auto modelo = zafra::identificarModelo(filePath.toStdString());
            // if (doc == zafra::NAO_IDENTIFICADO) {
                // Modelo não catalogado.
                qInfo() << "[ProcessFile] Documento possui uma estrutura nao suportada:" << filePath;
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