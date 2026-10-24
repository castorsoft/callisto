#pragma once

#include "api/IPlugin.h"

#include <QObject>
#include <QList>
#include <QPluginLoader>

class PluginManager: public QObject {
    Q_OBJECT

public:
    static PluginManager& instance();

    void loadPlugins(const QString &pluginsDir);
    QList<IPlugin*> plugins() const;
    IPlugin* pluginByName(const QString &name) const;

    void setContext(std::shared_ptr<IPluginContext> context);
    std::shared_ptr<IPluginContext> context() const { return m_context; }

private:
    PluginManager() = default;
    ~PluginManager();

    std::shared_ptr<IPluginContext> m_context;
    QList<QPluginLoader*> m_loaders;
    QList<IPlugin*> m_plugins;
};