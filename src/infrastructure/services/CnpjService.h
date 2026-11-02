#pragma once

#include "../../core/interfaces/ICnpjService.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

class CnpjService : public ICnpjService {
    Q_OBJECT
public:
    explicit CnpjService(QObject *parent = nullptr);
    ~CnpjService() override = default;

    void consultarCNPJ(const QString &cnpj) override;

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_networkManager;
};
