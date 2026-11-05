#include "SpedFiscalPlugin.h"
#include "ui/FormSpedFiscal.h"
#include <QAction>
#include <QDebug>

void SpedFiscalPlugin::initializePlugin() const
{
    qInfo() << "[SpedFiscalPlugin] Plugin SPED Fiscal inicializado.";
}

void SpedFiscalPlugin::initializeUI() const
{
    qInfo() << "[SpedFiscalPlugin] UI do SPED Fiscal inicializada.";
}

QAction* SpedFiscalPlugin::getAction(QObject *parent)
{
    auto *action = new QAction("SPED Fiscal", parent);
    action->setToolTip("Geração do arquivo digital SPED EFD ICMS/IPI");
    connect(action, &QAction::triggered, this, &SpedFiscalPlugin::onTriggered);
    return action;
}

void SpedFiscalPlugin::onTriggered()
{
    if (m_uiContext) {
        if (!m_form) {
            m_form = new FormSpedFiscal(m_uiContext);
        }
        m_uiContext->openTab(m_form, "SPED Fiscal");
    } else {
        qWarning() << "[SpedFiscalPlugin] Tentativa de abrir aba sem contexto UI configurado.";
    }
}
