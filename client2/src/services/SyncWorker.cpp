#include "SyncWorker.h"

#include <QThread>
#include <QTimer>

SyncWorker::SyncWorker(QObject *parent): IService(parent) {
    this->moveToThread(&m_thread);
}

SyncWorker::~SyncWorker()
{
    stopService();
}

bool SyncWorker::isRunning() const
{
    return m_isRunning;
}

void SyncWorker::startService()
{
    if (m_isRunning) return;

    connect(&m_thread, &QThread::started, this, [this]() {
        m_timer = new QTimer();
        connect(m_timer, &QTimer::timeout, this, &SyncWorker::processTask);
        m_timer->start(1000);

        m_isRunning = true;
        emit serviceStarted();
        qInfo() << "[SyncWorker] Serviço iniciado na Thread:" << QThread::currentThreadId();
    });

    m_thread.start();
}

void SyncWorker::stopService()
{
    if (!m_isRunning) return;

    if (m_timer) {
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }

    m_thread.quit();
    m_thread.wait();

    m_isRunning = false;
    emit serviceStopped();
    qInfo() << "[SyncWorker] Serviço parado.";
}