#pragma once

#include "IService.h"
#include "../../../core/entities/Arquivo.h"
#include <QThreadPool>

class ScheduleFilesForProcessing: public IService {
    Q_OBJECT

public:
    explicit ScheduleFilesForProcessing(QObject *parent = nullptr);
    ~ScheduleFilesForProcessing() override;

    QString serviceName() const override { return "Serviço de agendamento e pré-processamento de arquivos"; }
    bool isRunning() const override;

public slots:
    void startService() override;
    void stopService() override;
    void onFileAdded(const QString &filePath);

signals:
    void requestFileProcess(const QUuid &uuid);

private:
    void preProcessFile(const QString &filePath);

    bool m_isRunning{false};
    QThreadPool m_preProcessPool;
};
