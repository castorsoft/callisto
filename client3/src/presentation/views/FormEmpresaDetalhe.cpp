#include "FormEmpresaDetalhe.h"
#include "ui_FormEmpresaDetalhe.h"
#include "../components/ToastNotification.h"
#include "../../core/entities/Empresa.h"
#include "../../infrastructure/database/repositories/EmpresaRepository.h"
#include "../../infrastructure/database/repositories/EnderecoRepository.h"

FormEmpresaDetalhe::FormEmpresaDetalhe(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormEmpresaDetalhe) {
    ui->setupUi(this);

    ui->rgPessoa->addOption("Física");
    ui->rgPessoa->addOption("Jurídica");

    ui->cbRegimeTributario->addItem("SIMPLES NACIONAL", "1");
    ui->cbRegimeTributario->addItem("SN - EXCESSO DE RECEITA", "2");
    ui->cbRegimeTributario->addItem("REGIME NORMAL", "2");
    ui->cbRegimeTributario->addItem("MEI", "3");

    connect(ui->rgPessoa, &InputRadioGroup::currentIndexChanged, this, &FormEmpresaDetalhe::onPessoaChange);
    connect(ui->btnCancelar, &QPushButton::clicked, this, &FormEmpresaDetalhe::fechar);
    connect(ui->btnSalvar, &QPushButton::clicked, this, &FormEmpresaDetalhe::onSalvar);

    ui->rgPessoa->setCurrentIndex(0);
}

FormEmpresaDetalhe::~FormEmpresaDetalhe() {
    delete ui;
}

void FormEmpresaDetalhe::onSalvar() {
    bool isJuridica = ui->rgPessoa->currentIndex() == 1;
    QString cpfCnpj = isJuridica ? ui->edtCNPJ->text() : ui->edtCPF->text();
    QString razaoSocial = ui->edtRazaoSocial->text();
    QString nomeFantasia = ui->edtNomeFantasia->text();
    QString regimeTributario = ui->cbRegimeTributario->currentData().toString();

    // Limpeza da máscara
    cpfCnpj.remove('.').remove('-').remove('/');

    if (cpfCnpj.isEmpty() || razaoSocial.isEmpty()) {
        ToastNotification::show(this, "Aviso", "Preencha os campos obrigatórios (CPF/CNPJ e Razão Social).", ToastNotification::Warning, 3000);
        return;
    }

    try {
        if
        EnderecoRepository endRepo;


        Empresa emp(cpfCnpj, razaoSocial, nomeFantasia.isEmpty() ? std::nullopt : std::make_optional(nomeFantasia),
                    regimeTributario, 0, std::nullopt, std::nullopt);

        EmpresaRepository repo;
        repo.salvar(emp);

        ToastNotification::show(this, "Sucesso", "Empresa salva com sucesso!", ToastNotification::Success, 3000);
        emit fechar();
    } catch (const std::exception &e) {
        ToastNotification::show(this, "Erro", QString("Ocorreu um erro ao salvar a empresa:\n") + e.what(), ToastNotification::Error, 5000);
    }
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