#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "../../plugins/PluginManager.h"
#include "../plugins/api/IUIExtension.h"

#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    loadNativeButtons();
    loadPluginButtons();
    connect(ui->listWidget,&QListWidget::itemClicked, this, &MainWindow::onListClick);
}

MainWindow::~MainWindow() {
    delete ui;
}

QMdiSubWindow *MainWindow::addSubForm(QWidget *form) {
    if (!form) return nullptr;

    QMdiSubWindow *subWindow = ui->mdiArea->addSubWindow(form);
    form->setAttribute(Qt::WA_DeleteOnClose);

    subWindow->showMaximized();

    return subWindow;
}

void MainWindow::loadPluginButtons() {
    const QList<IPlugin*> plugins = PluginManager::instance().plugins();
    for (IPlugin *plugin: plugins) {
        if (auto uiExtension = dynamic_cast<IUIExtension*>(plugin)) {
            QAction *action = uiExtension->getAction(this);
            addActionToList(action);
        }
    }
}

void MainWindow::loadNativeButtons() {
    QAction *actionListarEmpresas = new QAction("Empresas", this);

    connect(actionListarEmpresas, &QAction::triggered, this, &MainWindow::onClickEmpresas);

    addActionToList(actionListarEmpresas);
}

void MainWindow::addActionToList(QAction *action) {
    if(!action) return;

    auto item = new QListWidgetItem(action->text(), ui->listWidget);
    item->setToolTip(action->toolTip());
    item->setIcon(action->icon());

    item->setData(Qt::UserRole, QVariant::fromValue(action));
}

void MainWindow::onListClick(QListWidgetItem* item) {
    if (!item) return;
    auto action = item->data(Qt::UserRole).value<QAction*>();
    if (action && action->isEnabled()) {
        action->trigger();
    }
}

void MainWindow::onClickEmpresas() {
    // auto form = new FormEmpresasListagem(this);

    // this->addSubForm(form);
}
