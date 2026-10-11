#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "../../plugins/PluginManager.h"
#include "../plugins/api/IUIExtension.h"

#include "empresas/FormEmpresas.h"
#include "pessoas/FormPessoas.h"
#include "documentos_fiscais/FormProcessarXml.h"

#include <QListWidgetItem>
#include <QDialog>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>

#include "empresas/FormEmpresaDetalhe.h"
#include "../components/ToastNotification.h"
#include "../../infrastructure/database/repositories/EmpresaRepository.h"
#include "../../core/GlobalState.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    loadNativeButtons();
    loadPluginButtons();
    connect(ui->listWidget,&QListWidget::itemClicked, this, &MainWindow::onListClick);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeSubForm);

    QTimer::singleShot(0, this, &MainWindow::verificarEmpresaInicial);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::verificarEmpresaInicial() {
    EmpresaRepository repo;
    QList<Empresa> empresas = repo.listarTodas();
    if (!empresas.isEmpty()) {
        GlobalState::instance().setEmpresaAtual(empresas.first());
        return;
    }

    QMessageBox::information(
        this,
        "Boas-vindas",
        "Seja bem-vindo ao sistema!\n\nPara iniciar a utilização, é necessário efetuar o cadastro da sua empresa."
    );

    QDialog dialog(this);
    dialog.setWindowTitle("Cadastro da Empresa");
    dialog.setModal(true);
    dialog.resize(600, 520);

    auto *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *formDetalhe = new FormEmpresaDetalhe(&dialog);
    layout->addWidget(formDetalhe);

    connect(formDetalhe, &FormDetalheBase::fechar, &dialog, &QDialog::accept);

    dialog.exec();

    // Caso o formulário tenha sido fechado sem salvar, encerra a aplicação
    if (!GlobalState::instance().empresaAtual().has_value()) {
        QList<Empresa> recarregadas = repo.listarTodas();
        if (!recarregadas.isEmpty()) {
            GlobalState::instance().setEmpresaAtual(recarregadas.first());
        } else {
            close();
            QApplication::quit();
            return;
        }
    }

    ToastNotification::show(this, "Sucesso", "Empresa cadastrada com sucesso! Bem-vindo.", ToastNotification::Success, 3000);
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

    QAction *actionListarPessoas = new QAction("Pessoas", this);
    connect(actionListarPessoas, &QAction::triggered, this, &MainWindow::onClickPessoas);

    addActionToList(actionListarPessoas);

    QAction *actionProcessarXml = new QAction("Processar XMLs", this);
    connect(actionProcessarXml, &QAction::triggered, this, &MainWindow::onClickProcessarXml);

    addActionToList(actionProcessarXml);
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

template <typename T>
bool MainWindow::existeFormulario() {
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        if (qobject_cast<T*>(ui->tabWidget->widget(i))) {
            ui->tabWidget->setCurrentIndex(i); // Apenas foca na aba existente
            return true;
        }
    }
    return false;
}

void MainWindow::onClickEmpresas() {
    if (existeFormulario<FormEmpresas>()) return;

    auto form = new FormEmpresas(this);
    form->setWindowTitle("Empresas");
    this->addSubForm(form);
}

void MainWindow::onClickPessoas() {
    if (existeFormulario<FormPessoas>()) return;

    auto form = new FormPessoas(this);
    form->setWindowTitle("Pessoas");
    this->addSubForm(form);
}

void MainWindow::onClickProcessarXml() {
    if (existeFormulario<FormProcessarXml>()) return;

    auto form = new FormProcessarXml(this);
    form->setWindowTitle("Processar XMLs");
    this->addSubForm(form);
}
