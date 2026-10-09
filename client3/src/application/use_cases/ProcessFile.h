#pragma once

#include "../../core/entities/Arquivo.h"

#include <zafra/document/documents/nfe_document.hpp>

#include <QObject>
#include <QThreadPool>

class ProcessFile: public QObject {
    Q_OBJECT

public:
    explicit ProcessFile(QObject *parent = nullptr);
    ~ProcessFile() override;

public slots:
    void execute(const QUuid &uuid);

signals:
    void processingStarted(const Arquivo &arquivo);
    void processingFinished(const Arquivo &arquivo, bool success);
    void fileError(const Arquivo &arquivo, const QString &errorMessage);

private:
    void processFileTask(const QUuid &uuid);
    void processarNFe(const zafra::document::NFeDocument &doc, const QString &filePath);
    int carregarOuCriarPessoa(const zafra::document::NFeDocument &doc, const QString &tipoEmissao);

    QThreadPool m_processPool;
};