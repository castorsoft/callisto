#pragma once

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

struct CnpjResult {
    QString cnpj;
    QString razaoSocial;
    QString nomeFantasia;
    QString regimeTributario;

    QString logradouro;
    QString numero;
    QString complemento;
    QString bairro;
    QString cep;
    int ibgeCidade = 0;
    QString siglaEstado;

    QString inscricaoEstadual;
    bool isValid = false;
    QString errorString;
};

class CnpjService : public QObject {
    Q_OBJECT
public:
    explicit CnpjService(QObject *parent = nullptr);

    void consultarCNPJ(const QString &cnpj);

signals:
    void consultaFinalizada(const CnpjResult &resultado);
    void erroConsulta(const QString &erro);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_networkManager;
};
