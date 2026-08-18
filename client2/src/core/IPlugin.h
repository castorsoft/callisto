#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QString>
#include <QtPlugin>

class IPlugin {
public:
    virtual ~IPlugin() = default;

    virtual QString name() const = 0;
    virtual QString version() const = 0;
    virtual void initialize() const = 0;
    virtual void deinitialize() const = 0;
};

#define IPlugin_iid "dev.castorsoft.Callisto.IPlugin/0.0.1-alpha"
Q_DECLARE_INTERFACE(IPlugin, IPlugin_iid)

#endif