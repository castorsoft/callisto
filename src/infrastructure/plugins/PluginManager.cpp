#include "PluginManager.h"

#include <QDir>
#include <QDebug>

PluginManager &PluginManager::instance()
{
    static PluginManager instance;
    return instance;
}

void PluginManager::loadPlugins(const QString &pluginsDir)
{
    QDir dir(pluginsDir);
    if (!dir.exists()) {
        qWarning() << "[PluginManager] Diretorio nao encontrado:" << pluginsDir;
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
                if (m_context) {
                    plugin->setContext(m_context);
                }
                plugin->initializePlugin();
                m_plugins.append(plugin);
                m_loaders.append(loader);
                qInfo() << "[PluginManager] carregado plugin" << plugin->name() << plugin->version();
            } else {
                loader->unload();
                delete loader;
                continue;
            }
        } else {
            qWarning() << "[PluginManager] Erro ao carregar os plugins:" << loader->errorString();
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

void PluginManager::setContext(std::shared_ptr<IPluginContext> context)
{
    m_context = context;
    for (auto plugin: m_plugins) {
        if (plugin) {
            plugin->setContext(m_context);
        }
    }
}

PluginManager::~PluginManager()
{
    for (auto loader: m_loaders) {
        loader->unload();
        delete loader;
    }
}

