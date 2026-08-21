#ifndef SYNCWORKER_H
#define SYNCWORKER_H

#include "../core/IService.h"
#include <QThread>
#include <QTimer>

class SyncWorker: public IService {
    Q_OBJECT

public:
    explicit SyncWorker(QObject *parent = nullptr);
    ~SyncWorker() override;

    QString serviceName() const override { return "Serviço de sincronização em segundo plano"; }
    bool isRunning() const override;

public slots:
    void startService() override;
    void stopService() override;

private slots:
    void processTask();

private:
    QThread m_thread;
    QTimer *m_timer{nullptr};
    bool m_isRunning{false};
};

#endif