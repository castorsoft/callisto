#include "CnpjService.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

CnpjService::CnpjService(QObject *parent) : QObject(parent) {
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &CnpjService::onReplyFinished);
}

void CnpjService::consultarCNPJ(const QString &cnpj) {
    QString cleanCnpj = cnpj;
    cleanCnpj.remove('.').remove('-').remove('/');

    if (cleanCnpj.isEmpty()) {
        emit erroConsulta("CNPJ vazio.");
        return;
    }

    QUrl url(QString("https://publica.cnpj.ws/cnpj/%1").arg(cleanCnpj));
    QNetworkRequest request(url);

    // Some APIs require a User-Agent
    request.setHeader(QNetworkRequest::UserAgentHeader, "CastorSoft DFE Client/1.0");

    m_networkManager->get(request);
}

void CnpjService::onReplyFinished(QNetworkReply *reply) {
    CnpjResult result;
    result.isValid = false;

    if (reply->error() != QNetworkReply::NoError) {
        result.errorString = reply->errorString();
        emit erroConsulta(QString("Erro na requisição: %1").arg(reply->errorString()));
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        result.errorString = "Erro ao ler resposta da API (JSON inválido).";
        emit erroConsulta(result.errorString);
        reply->deleteLater();
        return;
    }

    if (!jsonDoc.isObject()) {
        result.errorString = "Resposta da API não é um objeto JSON.";
        emit erroConsulta(result.errorString);
        reply->deleteLater();
        return;
    }

    QJsonObject rootObj = jsonDoc.object();

    // Check for API error structure if exists
    if (rootObj.contains("status") && rootObj["status"].toInt() == 400) {
        result.errorString = rootObj["detalhes"].toString();
        emit erroConsulta(result.errorString);
        reply->deleteLater();
        return;
    }

    result.razaoSocial = rootObj["razao_social"].toString();

    // Simples Nacional status (often under "simples")
    // Note: Depends on what the API returns. We can put "SIMPLES NACIONAL" if simple is true, etc.
    QJsonValue simplesObj = rootObj["simples"];
    if (simplesObj.isObject()) {
        QString optante = simplesObj.toObject()["simples"].toString();
        if (optante == "Sim") {
            result.regimeTributario = "1"; // SIMPLES NACIONAL
        }
    }

    QJsonObject estabelecimento = rootObj["estabelecimento"].toObject();
    if (!estabelecimento.isEmpty()) {
        result.cnpj = estabelecimento["cnpj"].toString();
        result.nomeFantasia = estabelecimento["nome_fantasia"].toString();

        QString tipoLogradouro = estabelecimento["tipo_logradouro"].toString();
        QString logradouro = estabelecimento["logradouro"].toString();
        if (!tipoLogradouro.isEmpty()) {
            result.logradouro = tipoLogradouro + " " + logradouro;
        } else {
            result.logradouro = logradouro;
        }

        result.numero = estabelecimento["numero"].toString();
        result.complemento = estabelecimento["complemento"].toString();
        result.bairro = estabelecimento["bairro"].toString();
        result.cep = estabelecimento["cep"].toString();

        QJsonObject cidade = estabelecimento["cidade"].toObject();
        if (!cidade.isEmpty()) {
            result.ibgeCidade = cidade["ibge_id"].toInt();
        }

        QJsonObject estado = estabelecimento["estado"].toObject();
        if (!estado.isEmpty()) {
            result.siglaEstado = estado["sigla"].toString();
        }

        QJsonArray inscricoes = estabelecimento["inscricoes_estaduais"].toArray();
        for (const QJsonValue &val : inscricoes) {
            QJsonObject ie = val.toObject();
            if (ie["ativo"].toBool()) {
                result.inscricaoEstadual = ie["inscricao_estadual"].toString();
                break; // Take the first active IE
            }
        }
    }

    result.isValid = true;
    emit consultaFinalizada(result);
    reply->deleteLater();
}
