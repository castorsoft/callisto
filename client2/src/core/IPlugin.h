#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QString>
#include <QtPlugin>
#include <QAction>

class IPlugin {
public:
    virtual ~IPlugin() = default;

    // Métodos para a identificacão do plugin
    virtual QString name() const = 0;       // Nome único
    virtual QString version() const = 0;    // Versão atual do plugin

    // Método para criar um componente clicável
    virtual QAction* getAction(QObject *parent = nullptr) = 0;

    virtual void initialize() const = 0;
};

#define IPlugin_iid "dev.castorsoft.Callisto.IPlugin/0.0.1-alpha"
Q_DECLARE_INTERFACE(IPlugin, IPlugin_iid)

#endif