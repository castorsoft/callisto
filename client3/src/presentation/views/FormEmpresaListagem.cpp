#include "formEmpresaListagem.h"
#include "ui_FormEmpresaListagem.h"

FormEmpresaListagem::FormEmpresaListagem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormEmpresaListagem)
{
    ui->setupUi(this);
}

FormEmpresaListagem::~FormEmpresaListagem()
{
    delete ui;
}
