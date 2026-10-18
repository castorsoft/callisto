#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "../../plugins/PluginManager.h"
#include "../plugins/api/IUIExtension.h"

#include "empresas/FormEmpresas.h"
#include "pessoas/FormPessoas.h"
#include "produtos/FormProdutos.h"
#include "documentos_fiscais/FormProcessarXml.h"
#include "documentos_fiscais/FormVincularProdutos.h"

#include <QListWidgetItem>
#include <QDialog>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>
#include <QPainter>
#include <QPalette>

#include "empresas/FormEmpresaDetalhe.h"
#include "../components/ToastNotification.h"
#include "../../infrastructure/database/repositories/EmpresaRepository.h"
#include "../../core/GlobalState.h"

namespace {

QPixmap renderColoredPixmap(const QString &resourcePath, const QSize &size, const QColor &color) {
    QPixmap pixmap = QIcon(resourcePath).pixmap(size);
    if (pixmap.isNull()) return QPixmap();

    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), color);
    painter.end();

    return pixmap;
}

QIcon createThemedIcon(const QString &resourcePath, const QPalette &palette, const QSize &size = QSize(20, 20)) {
    QIcon icon;

    icon.addPixmap(renderColoredPixmap(resourcePath, size, palette.color(QPalette::Text)), QIcon::Normal);

    icon.addPixmap(renderColoredPixmap(resourcePath, size, palette.color(QPalette::HighlightedText)), QIcon::Selected);
    return icon;
}

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->listWidget->setIconSize(QSize(20, 20));
    ui->listWidget->setSpacing(2);

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
    const QPalette &pal = ui->listWidget->palette();

    QAction *actionListarEmpresas = new QAction(createThemedIcon("://icons/empresa.svg", pal), "Empresas", this);
    connect(actionListarEmpresas, &QAction::triggered, this, &MainWindow::onClickEmpresas);

    addActionToList(actionListarEmpresas);

    QAction *actionListarPessoas = new QAction(createThemedIcon("://icons/pessoa.svg", pal), "Pessoas", this);
    connect(actionListarPessoas, &QAction::triggered, this, &MainWindow::onClickPessoas);

    addActionToList(actionListarPessoas);

    QAction *actionListarProdutos = new QAction(createThemedIcon("://icons/produto.svg", pal), "Produtos", this);
    connect(actionListarProdutos, &QAction::triggered, this, &MainWindow::onClickProdutos);

    addActionToList(actionListarProdutos);

    QAction *actionProcessarXml = new QAction("Processar XMLs", this);
    connect(actionProcessarXml, &QAction::triggered, this, &MainWindow::onClickProcessarXml);

    addActionToList(actionProcessarXml);

    QAction *actionVincularProdutos = new QAction(createThemedIcon("://icons/vincular.svg", pal), "Vincular Produtos de Terceiros", this);
    connect(actionVincularProdutos, &QAction::triggered, this, &MainWindow::onClickVincularProdutos);

    addActionToList(actionVincularProdutos);
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

void MainWindow::onClickProdutos() {
    if (existeFormulario<FormProdutos>()) return;

    auto form = new FormProdutos(this);
    form->setWindowTitle("Produtos");
    this->addSubForm(form);
}

void MainWindow::onClickProcessarXml() {
    if (existeFormulario<FormProcessarXml>()) return;

    auto form = new FormProcessarXml(this);
    form->setWindowTitle("Processar XMLs");
    this->addSubForm(form);
}

void MainWindow::onClickVincularProdutos() {
    if (existeFormulario<FormVincularProdutos>()) return;

    auto form = new FormVincularProdutos(this);
    form->setWindowTitle("Vincular Produtos de Terceiros");
    this->addSubForm(form);
}
