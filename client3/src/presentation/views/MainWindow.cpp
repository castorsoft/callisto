#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "../../plugins/PluginManager.h"
#include "../plugins/api/IUIExtension.h"

#include "FormEmpresaListagem.h"

#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    loadNativeButtons();
    loadPluginButtons();
    connect(ui->listWidget,&QListWidget::itemClicked, this, &MainWindow::onListClick);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeSubForm);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addSubForm(QWidget *form) {
    if (!form) return;

    form->setAttribute(Qt::WA_DeleteOnClose);
    int index = ui->tabWidget->addTab(form, form->windowTitle());
    ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::closeSubForm(int index) {
    QWidget *form = ui->tabWidget->widget(index);
    if (form) {
        form->close();
    }
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
    // Verifica se o formulário já está aberto
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        if (qobject_cast<FormEmpresaListagem*>(ui->tabWidget->widget(i))) {
            ui->tabWidget->setCurrentIndex(i); // Apenas foca na aba existente
            return;
        }
    }

    auto form = new FormEmpresaListagem(this);
    this->addSubForm(form);
}
