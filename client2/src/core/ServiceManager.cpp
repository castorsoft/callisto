#include "ServiceManager.h"
#include <QDebug>

ServiceManager &ServiceManager::instance()
{
    static ServiceManager instance;
    return instance;
}

ServiceManager::~ServiceManager()
{
    stopAll();
}

void ServiceManager::RegisterService(IService *service)
{
    if (service && !m_services.contains(service)) {
        m_services.append(service);

        qInfo() << "[ServiceManager] Servico registrado:" << service->serviceName();
    }
}

void ServiceManager::startAll()
{
    for (auto service: m_services) {
        if (!service->isRunning() && !m_threads.contains(service)) {
            QThread *thread = new QThread();
            m_threads.insert(service, thread);

            service->moveToThread(thread);

            // Quando a thread iniciar, ela invoca o startService() no contexto da nova thread
            QObject::connect(thread, &QThread::started, service, &IService::startService);

            thread->start();
        }
    }
}

void ServiceManager::stopAll()
{
    for (auto service: m_services) {
        if (service->isRunning()) {
            QMetaObject::invokeMethod(service, "stopService", Qt::BlockingQueuedConnection);
        }

        if (m_threads.contains(service)) {
            QThread *thread = m_threads.take(service);
            thread->quit();
            thread->wait();
            delete thread;
        }
    }
}

QList<IService *> ServiceManager::activeServices() const
{
    return m_services;
}
