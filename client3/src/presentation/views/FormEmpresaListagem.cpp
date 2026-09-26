#include "FormEmpresaListagem.h"
#include "ui_FormEmpresaListagem.h"
#include "FormEmpresaDetalhe.h"
#include "../../infrastructure/database/repositories/EmpresaRepository.h"
#include "providers/EmpresaListProvider.h"
#include "../components/providers/TableDataProviderAdapter.h"

FormEmpresaListagem::FormEmpresaListagem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormEmpresaListagem)
{
    ui->setupUi(this);

    // Conecta o sinal do botão "Novo" da listagem
    connect(ui->genericListWidget, &GenericListWidget::actionNewRequested, this, &FormEmpresaListagem::onNovoClicked);
    // connect(ui->genericListWidget, &GenericListWidget::actionEditRequested, this, &FormEmpresaDetalhe::onAlterarClicked);

    carregarLista();
}

FormEmpresaListagem::~FormEmpresaListagem()
{
    delete ui;
}

void FormEmpresaListagem::onNovoClicked()
{
    auto detalhe = new FormEmpresaDetalhe(this);

    // IMPORTANTE: Aqui você pode conectar os sinais do seu detalhe (ex: clicou em cancelar ou salvar)
    // para fechar o formulário e voltar pra tabela. Exemplo fictício:
    connect(detalhe, &FormEmpresaDetalhe::fechar, this, &FormEmpresaListagem::voltarParaListagem);

    ui->stackedWidget->addWidget(detalhe);
    ui->stackedWidget->setCurrentWidget(detalhe);
}

void FormEmpresaListagem::onAlterarClicked(int rowIndex) {

}

void FormEmpresaListagem::voltarParaListagem()
{
    QWidget* widgetAtual = ui->stackedWidget->currentWidget();
    ui->stackedWidget->setCurrentIndex(0);

    if (widgetAtual != ui->pageListagem) {
        ui->stackedWidget->removeWidget(widgetAtual);
        widgetAtual->deleteLater();
    }

    carregarLista();
}

void FormEmpresaListagem::carregarLista()
{
    EmpresaRepository repo;
    auto empresas = repo.listarTodas();
    auto provider = std::make_shared<EmpresaListProvider>(empresas);
    auto model = new TableDataProviderAdapter(provider, this);
    ui->genericListWidget->setModel(model);
}
