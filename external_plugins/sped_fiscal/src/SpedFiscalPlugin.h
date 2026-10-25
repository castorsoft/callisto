#pragma once

#include <QObject>
#include <QPointer>
#include <memory>
#include "../../../src/plugins/api/IPlugin.h"
#include "../../../src/presentation/plugins/api/IUIExtension.h"
#include "ui/FormSpedFiscal.h"

class SpedFiscalPlugin : public QObject, public IPlugin, public IUIExtension {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dev.castorsoft.Callisto.IPlugin/0.0.1-alpha" FILE "plugin_metadata.json")
    Q_INTERFACES(IPlugin)

public:
    QString name() const override { return "dev.castorsoft.sped-fiscal"; }
    QString version() const override { return "1.0.0"; }

    void setContext(std::shared_ptr<IPluginContext> context) override {
        m_context = context;
    }

    void initializePlugin() const override;
    void initializeUI() const override;

    QAction* getAction(QObject *parent = nullptr) override;
    QString iconPath() const override { return "://icons/sped.svg"; }

private slots:
    void onTriggered();

private:
    std::shared_ptr<IPluginContext> m_context;
    QPointer<FormSpedFiscal> m_form;
};
