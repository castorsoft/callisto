#pragma once

#include <QString>
#include <QtPlugin>
#include <memory>

class IPluginContext;

class IPlugin {
public:
    virtual ~IPlugin() = default;

    // Métodos para a identificação do plugin
    virtual QString name() const = 0;       // Nome único
    virtual QString version() const = 0;    // Versão atual do plugin

    virtual void setContext(std::shared_ptr<IPluginContext> /*context*/) {}
    virtual void initializePlugin() const = 0;
};

#define IPlugin_iid "dev.castorsoft.Callisto.IPlugin/0.0.1-alpha"
Q_DECLARE_INTERFACE(IPlugin, IPlugin_iid)

