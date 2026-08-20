#include "PluginManager.h"

#include <QDir>

PluginManager &PluginManager::instance()
{
    static PluginManager instance;
    return instance;
}

void PluginManager::loadPlugins(const QString &pluginsDir)
{
    QDir dir(pluginsDir);
    if (!dir.exists()) {
        qWarning() << "[PluginManager] Diretorio nao encontrado: " << pluginsDir;
        return;
    }

    for (const QString &fileName: dir.entryList(QDir::Files)) {
        // TODO: Usar variáveis para identificar se está compilando para win ou linux
        if (!fileName.endsWith(".dll") && !fileName.endsWith(".so")) {
            continue;
        }

        auto loader = new QPluginLoader(dir.absoluteFilePath(fileName), this);
        QObject *pluginInstance = loader->instance();

        if (pluginInstance) {
            auto plugin = qobject_cast<IPlugin*>(pluginInstance);
            if (plugin) {
                plugin->initialize();
                m_plugins.append(plugin);
                m_loaders.append(loader);
                qInfo() << "[PluginManager] carregado plugin " << plugin->name() << " v" << plugin->version();
            } else {
                loader->unload();
                delete loader;
            }
        } else {
            qWarning() << "[PluginManager] Erro ao carregar os plugins: " << loader->errorString();
            delete loader;
        }
    }
}

QList<IPlugin *> PluginManager::plugins() const
{
    return m_plugins;
}

IPlugin *PluginManager::pluginByName(const QString &name) const
{
    for (auto plugin: m_plugins) {
        if (plugin->name() == name) return plugin;
    }
    return nullptr;
}
