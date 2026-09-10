#include "TestButtonPlugin.h"

QAction* TestButtonPlugin::getAction(QObject *parent)
{
    QAction* action = new QAction("Menu teste", parent);
    action->setToolTip("Descrição");

    connect(action, &QAction::triggered, this, &TestButtonPlugin::onTrigger);

    return action;
}

void TestButtonPlugin::onTrigger() {
    qInfo() << "[TestButtonPlugin] Acao disparada no plugin";
}