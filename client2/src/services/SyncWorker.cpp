#include "SyncWorker.h"

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
