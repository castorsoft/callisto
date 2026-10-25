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
    if (m_context) {
        if (!m_form) {
            m_form = new FormSpedFiscal(m_context);
        }
        m_context->openTab(m_form, "SPED Fiscal");
    } else {
        qWarning() << "[SpedFiscalPlugin] Tentativa de abrir aba sem contexto configurado.";
    }
}
