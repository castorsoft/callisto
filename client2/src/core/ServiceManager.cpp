#include "ServiceManager.h"

ServiceManager &ServiceManager::instance()
{
    static ServiceManager instance;
    return instance;
}