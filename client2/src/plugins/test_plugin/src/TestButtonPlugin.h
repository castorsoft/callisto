#ifndef TESTBUTTONPLUGIN_H
#define TESTBUTTONPLUGIN_H

#include <QObject>
#include "../../../infrastructure/plugins/IPlugin.h"

class TestButtonPlugin: public QObject, public IPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dev.castorsoft.Callisto.IPlugin/0.0.1-alpha" FILE "plugin_metadata.json")
    Q_INTERFACES(IPlugin)

public:
    QString name() const override { return "dev.castorsoft.teste-button"; }
    QString version() const override { return "0.0.1"; }

    QAction* getAction(QObject *parent = nullptr) override;

    void initialize() const override { qInfo() << "[TestButtonPlugin] Inicializado"; }
private slots:

    void onTrigger();
};

#endif