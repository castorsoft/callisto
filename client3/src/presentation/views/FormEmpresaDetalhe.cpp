#include "FormEmpresaDetalhe.h"
#include "ui_FormEmpresaDetalhe.h"
#include "../components/ToastNotification.h"
#include "../../core/entities/Empresa.h"
#include "../../infrastructure/database/repositories/EmpresaRepository.h"
#include "../../infrastructure/database/repositories/EnderecoRepository.h"
#include "../../infrastructure/database/repositories/EstadoRepository.h"
#include "../../infrastructure/database/repositories/CidadeRepository.h"
#include <QVariant>

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

    preencherUF();

    connect(ui->rgPessoa, &InputRadioGroup::currentIndexChanged, this, &FormEmpresaDetalhe::onPessoaChange);
    connect(ui->btnCancelar, &QPushButton::clicked, this, &FormEmpresaDetalhe::fechar);
    connect(ui->btnSalvar, &QPushButton::clicked, this, &FormEmpresaDetalhe::onSalvar);
    connect(ui->cbUF, &InputComboBox::currentIndexChanged, this, &FormEmpresaDetalhe::atualizarCidades);


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

    QString enderecoLogradouro = ui->edtLogradouro->text();
    QString enderecoNumero = ui->edtNumero->text();
    QString enderecoComplemento = ui->edtComplemento->text();
    QString enderecoBairro = ui->edtBairro->text();
    QString enderecoCEP = ui->edtCEP->text();
    int enderecoCidade = ui->cbCidade->currentData().toInt();

    // Limpeza da máscara
    cpfCnpj.remove('.').remove('-').remove('/');

    if (cpfCnpj.isEmpty() || razaoSocial.isEmpty()) {
        ToastNotification::show(this, "Aviso", "Preencha os campos obrigatórios (CPF/CNPJ e Razão Social).", ToastNotification::Warning, 3000);
        return;
    }

    try {
        Endereco endereco(enderecoLogradouro, enderecoNumero.isEmpty() ? std::nullopt : std::make_optional(enderecoNumero), enderecoComplemento.isEmpty() ? std::nullopt : std::make_optional(enderecoComplemento), enderecoBairro, enderecoCEP, enderecoCidade);

        EnderecoRepository endRepo;


        Empresa emp(cpfCnpj, razaoSocial, nomeFantasia.isEmpty() ? std::nullopt : std::make_optional(nomeFantasia), regimeTributario, endereco, std::nullopt, std::nullopt);

        EmpresaRepository repo;
        // repo.salvar(emp);

        ToastNotification::show(this, "Sucesso", "Empresa salva com sucesso!", ToastNotification::Success, 3000);
        emit fechar();
    } catch (const std::exception &e) {
        ToastNotification::show(this, "Erro", QString("Ocorreu um erro ao salvar a empresa:\n") + e.what(), ToastNotification::Error, 5000);
    }
}

void FormEmpresaDetalhe::preencherUF() {
    ui->cbUF->clear();
    EstadoRepository repo;
    QList<Estado> estados = repo.listarTodos();
    for (const Estado& estado : estados) {
        ui->cbUF->addItem(estado.sigla(), estado.id());
    }
}

void FormEmpresaDetalhe::atualizarCidades(int indexUF) {
    ui->cbCidade->clear();

    QVariant ufData = ui->cbUF->comboBox()->itemData(indexUF);
    if (!ufData.isValid()) return;

    int idEstado = ufData.toInt();

    CidadeRepository repo;
    QList<Cidade> cidades = repo.listarPorEstado(idEstado);
    for (const Cidade& cidade : cidades) {
        ui->cbCidade->addItem(cidade.nome(), cidade.id());
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