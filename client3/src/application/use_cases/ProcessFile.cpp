#include "ProcessFile.h"

#include <QFileInfo>
#include "../../core/entities/Arquivo.h"
#include <zafra/document/document_factory.hpp>
#include <zafra/xml/xml_document.hpp>
#include <optional>

ProcessFile::ProcessFile(QObject *parent): QObject(parent) { }

void ProcessFile::execute(const Arquivo &modelArquivo)
{
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
                    processarNFe(static_cast<const zafra::document::NFeDocument&>(*doc));
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

#include "../../infrastructure/database/repositories/DocumentoFiscalRepository.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>

void ProcessFile::processarNFe(const zafra::document::NFeDocument &doc)
{
    qInfo() << "[ProcessarNFe] << Processando NFe" << QString::fromStdString(doc.getChaveAcesso());

    DocumentoFiscalRepository repo;

    if (repo.existeDocumento(QString::fromStdString(doc.getChaveAcesso()))) {
        qInfo() << "[ProcessarNFe] Documento ja existe no banco de dados.";
        return;
    }

    // TODO: Obter idPessoa do contexto do usuario atual. Hardcoded 1 por enquanto.
    int idPessoa = 1;

    QString chaveAcesso = QString::fromStdString(doc.getChaveAcesso());
    QString tipoOperacao = doc.getTipoOperacao() == zafra::core::TipoOperacao::Saida ? "1" : "0";

    QString tipoEmissao = "1"; // Padrao normal
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

        QJsonObject dadosItem;

        auto imposto = det->GetImposto();
        if (imposto) {
            QJsonObject impostosJson;

            if (imposto->GetICMS().has_value()) {
                QJsonObject icmsObj;
                auto icms = imposto->GetICMS().value();
                if (icms) {
                    if (icms->GetCST().has_value()) {
                        icmsObj["cst"] = QString::fromStdString(icms->GetCST().value());
                    } else if (icms->GetCSOSN().has_value()) {
                        icmsObj["csosn"] = QString::fromStdString(icms->GetCSOSN().value());
                    }

                    if (icms->GetVBC().has_value()) {
                        icmsObj["vBC"] = QString::number(icms->GetVBC().value(), 'f', 4);
                    }
                    if (icms->GetPICMS().has_value()) {
                        icmsObj["pICMS"] = QString::number(icms->GetPICMS().value(), 'f', 4);
                    }
                    if (icms->GetVICMS().has_value()) {
                        icmsObj["vICMS"] = QString::number(icms->GetVICMS().value(), 'f', 4);
                    }

                    impostosJson["icms"] = icmsObj;
                }
            }
            // if (impostoNode->GetChild("PIS")) {
            //     QJsonObject pisObj;
            //     impostosJson["pis"] = pisObj;
            // }
            // if (impostoNode->GetChild("COFINS")) {
            //     QJsonObject cofinsObj;
            //     impostosJson["cofins"] = cofinsObj;
            // }
            // if (impostoNode->GetChild("IPI")) {
            //     QJsonObject ipiObj;
            //     impostosJson["ipi"] = ipiObj;
            // }
            dadosItem["impostos"] = impostosJson;
        }
    }

    // // Iterar os itens usando IXmlNode, já que Zafra NFeDocument não expõe GetDet() no momento.
    // // Assim que a zafra for atualizada com o getter, podemos refatorar para doc.GetDet().
    // if (rootNode) {
    //     auto nfeNode = rootNode->GetChild("NFe");
    //     if (!nfeNode) nfeNode = rootNode->GetChild("nfeProc"); // fallback para nfeProc

    //     if (nfeNode) {
    //         auto infNfe = nfeNode->GetName() == "nfeProc" ? nfeNode->GetChild("NFe")->GetChild("infNFe") : nfeNode->GetChild("infNFe");
    //         if (infNfe) {
    //             auto detNodes = infNfe->GetChildren("det");
    //             for (const auto& detNode : detNodes) {
    //                 int nItem = std::stoi(detNode->GetAttribute("nItem").value_or("0"));

    //                 auto prodNode = detNode->GetChild("prod");
    //                 if (!prodNode) continue;

    //                 QString cProd = QString::fromStdString(prodNode->GetChild("cProd") ? prodNode->GetChild("cProd")->GetValue() : "");
    //                 QString xProd = QString::fromStdString(prodNode->GetChild("xProd") ? prodNode->GetChild("xProd")->GetValue() : "");
    //                 QString ncm = QString::fromStdString(prodNode->GetChild("NCM") ? prodNode->GetChild("NCM")->GetValue() : "");
    //                 QString uCom = QString::fromStdString(prodNode->GetChild("uCom") ? prodNode->GetChild("uCom")->GetValue() : "");

    //                 std::optional<int> cfop;
    //                 if (auto cfopNode = prodNode->GetChild("CFOP")) {
    //                     cfop = std::stoi(cfopNode->GetValue());
    //                 }

    //                 double qCom = prodNode->GetChild("qCom") ? std::stod(prodNode->GetChild("qCom")->GetValue()) : 0.0;
    //                 double vUnCom = prodNode->GetChild("vUnCom") ? std::stod(prodNode->GetChild("vUnCom")->GetValue()) : 0.0;
    //                 double vProd = prodNode->GetChild("vProd") ? std::stod(prodNode->GetChild("vProd")->GetValue()) : 0.0;
    //                 double vDesc = prodNode->GetChild("vDesc") ? std::stod(prodNode->GetChild("vDesc")->GetValue()) : 0.0;

    //                 QJsonObject dadosItem;

    //                 // Extraindo Impostos basicos
    //                 auto impostoNode = detNode->GetChild("imposto");
    //                 if (impostoNode) {
    //                     QJsonObject impostosJson;
    //                     // O parsing completo do imposto exige iterar as tags especificas (ICMS00, ICMS10, etc)
    //                     // Para este MVP, deixamos a estrutura pronta no JSON
    //                     if (impostoNode->GetChild("ICMS")) {
    //                         QJsonObject icmsObj;
    //                         // icmsObj["cst"] = ...;
    //                         // icmsObj["vBC"] = ...;
    //                         impostosJson["icms"] = icmsObj;
    //                     }
    //                     if (impostoNode->GetChild("PIS")) {
    //                         QJsonObject pisObj;
    //                         impostosJson["pis"] = pisObj;
    //                     }
    //                     if (impostoNode->GetChild("COFINS")) {
    //                         QJsonObject cofinsObj;
    //                         impostosJson["cofins"] = cofinsObj;
    //                     }
    //                     if (impostoNode->GetChild("IPI")) {
    //                         QJsonObject ipiObj;
    //                         impostosJson["ipi"] = ipiObj;
    //                     }
    //                     dadosItem["impostos"] = impostosJson;
    //                 }

    //                 DocumentoFiscalItem item(0, nItem, cProd, xProd, ncm, cfop, uCom, qCom, vUnCom, vProd, vDesc, dadosItem);
    //                 documento.adicionarItem(item);
    //             }
    //         }
    //     }
    // }

    if (repo.salvar(documento)) {
        qInfo() << "[ProcessarNFe] NFe salva com sucesso. ID:" << documento.id().value_or(0);
    } else {
        qCritical() << "[ProcessarNFe] Falha ao salvar NFe.";
    }
}