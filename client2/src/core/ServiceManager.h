#ifndef SERVICEMANAGER_H
#define SERVICEMANAGER_H

class ServiceManager {

public:
    static ServiceManager& instance();
private:
    ServiceManager() = default;
    ~ServiceManager() = default;

    ServiceManager(const ServiceManager&) = delete;
    ServiceManager& operator=(const ServiceManager&) = delete;
};

#endif