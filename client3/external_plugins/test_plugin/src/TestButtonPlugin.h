#pragma once

#include <QObject>
#include "../../../src/plugins/api/IPlugin.h"
#include "../../../src/presentation/plugins/api/IUIExtension.h"

class TestButtonPlugin: public QObject, public IPlugin, public IUIExtension {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dev.castorsoft.Callisto.IPlugin/0.0.1-alpha" FILE "plugin_metadata.json")
    Q_INTERFACES(IPlugin)

public:
    QString name() const override { return "dev.castorsoft.teste-button"; }
    QString version() const override { return "0.0.1"; }

    QAction* getAction(QObject *parent = nullptr) override;

    void initializePlugin() const override { qInfo() << "[TestButtonPlugin] Inicializado"; }
    void initializeUI() const override { qInfo() << "[TestButtonPlugin] UI Inicializado"; }
private slots:

    void onTrigger();
};