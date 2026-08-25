#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

#include "IService.h"
#include <QList>
#include <QHash>
#include <QThread>

class ServiceManager {

public:
    static ServiceManager& instance();

    void RegisterService(IService *service);
    void startAll();
    void stopAll();

    QList<IService*> activeServices() const;
private:
    ServiceManager() = default;
    ~ServiceManager();

    ServiceManager(const ServiceManager&) = delete;
    ServiceManager& operator=(const ServiceManager&) = delete;

    QList<IService*> m_services;
    QHash<IService*, QThread*> m_threads;
};

#endif