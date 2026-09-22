#include "FormEmpresaDetalhe.h"
#include "ui_FormEmpresaDetalhe.h"

FormEmpresaDetalhe::FormEmpresaDetalhe(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormEmpresaDetalhe) {
    ui->setupUi(this);

    ui->rgPessoa->addOption("Física");
    ui->rgPessoa->addOption("Jurídica");

    connect(ui->rgPessoa, &InputRadioGroup::currentIndexChanged, this, &FormEmpresaDetalhe::onPessoaChange);

    ui->rgPessoa->setCurrentIndex(0);
}

FormEmpresaDetalhe::~FormEmpresaDetalhe() {
    delete ui;
}

void FormEmpresaDetalhe::onPessoaChange(int index) {
    switch (index) {
    case 0:
        ui->edtCNPJ->setEnabled(false);
        ui->edtCPF->setEnabled(true);
        break;
    case 1:
        ui->edtCNPJ->setEnabled(true);
        ui->edtCPF->setEnabled(false);
    }
}