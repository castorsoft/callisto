#include "FormEmpresaListagem.h"
#include "ui_FormEmpresaListagem.h"
#include "FormEmpresaDetalhe.h"

FormEmpresaListagem::FormEmpresaListagem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormEmpresaListagem)
{
    ui->setupUi(this);

    // Conecta o sinal do botão "Novo" da listagem
    connect(ui->genericListWidget, &GenericListWidget::actionNewRequested, this, &FormEmpresaListagem::onNovoClicked);
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
    // connect(detalhe, &FormEmpresaDetalhe::fechar, this, &FormEmpresaListagem::voltarParaListagem);

    ui->stackedWidget->addWidget(detalhe);
    ui->stackedWidget->setCurrentWidget(detalhe);
}

void FormEmpresaListagem::voltarParaListagem()
{
    QWidget* widgetAtual = ui->stackedWidget->currentWidget();
    ui->stackedWidget->setCurrentIndex(0);

    if (widgetAtual != ui->pageListagem) {
        ui->stackedWidget->removeWidget(widgetAtual);
        widgetAtual->deleteLater();
    }
}
