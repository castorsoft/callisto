#include "FormEmpresaDetalhe.h"
#include "ui_FormEmpresaDetalhe.h"

FormEmpresaDetalhe::FormEmpresaDetalhe(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormEmpresaDetalhe)
{
    ui->setupUi(this);
}

FormEmpresaDetalhe::~FormEmpresaDetalhe()
{
    delete ui;
}
