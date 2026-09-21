#pragma once

#include "../../core/entities/Arquivo.h"

#include <zafra/document/documents/nfe_document.hpp>

#include <QObject>

class ProcessFile: public QObject {
    Q_OBJECT

public:
    explicit ProcessFile(QObject *parent = nullptr);

public slots:
    void execute(const QUuid &uuid);

signals:
    void processingStarted(const Arquivo &arquivo);
    void processingFinished(const Arquivo &arquivo, bool success);
    void fileError(const Arquivo &arquivo, const QString &errorMessage);
private:
    void processarNFe(const zafra::document::NFeDocument &doc);
};