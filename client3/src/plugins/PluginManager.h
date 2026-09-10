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

private:
    PluginManager() = default;
    ~PluginManager();

    QList<QPluginLoader*> m_loaders;
    QList<IPlugin*> m_plugins;
};