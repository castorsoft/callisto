#include "FormEmpresaDetalhe.h"
#include "ui_FormEmpresaDetalhe.h"
#include "../../components/ToastNotification.h"
#include "../../../core/entities/Empresa.h"
#include "../../../infrastructure/database/repositories/EmpresaRepository.h"
#include "../../../infrastructure/database/repositories/EnderecoRepository.h"
#include "../../../infrastructure/database/repositories/EstadoRepository.h"
#include "../../../infrastructure/database/repositories/CidadeRepository.h"
#include "../../../core/GlobalState.h"
#include "../../../core/constants/Mensagens.h"
#include <QVariant>

FormEmpresaDetalhe::FormEmpresaDetalhe(QWidget *parent, ICnpjService *cnpjService)
    : FormDetalheBase(parent)
    , ui(new Ui::FormEmpresaDetalhe)
    , m_cnpjService(cnpjService) {
    ui->setupUi(this);

    ui->rgPessoa->addOption("Física");
    ui->rgPessoa->addOption("Jurídica");

    ui->cbRegimeTributario->addItem("SIMPLES NACIONAL", "1");
    ui->cbRegimeTributario->addItem("SN - EXCESSO DE RECEITA", "2");
    ui->cbRegimeTributario->addItem("REGIME NORMAL", "2");
    ui->cbRegimeTributario->addItem("MEI", "3");

    preencherUF();
    atualizarCidades(1);

    connect(ui->edtCNPJ, &InputMasked::editingFinished, this, &FormEmpresaDetalhe::onCnpjEditingFinished);

    connect(ui->rgPessoa, &InputRadioGroup::currentIndexChanged, this, &FormEmpresaDetalhe::onPessoaChange);
    connect(ui->btnCancelar, &InputButton::clicked, this, &FormEmpresaDetalhe::fechar);
    connect(ui->btnSalvar, &InputButton::clicked, this, &FormEmpresaDetalhe::onSalvar);
    connect(ui->cbUF, &InputComboBox::currentIndexChanged, this, &FormEmpresaDetalhe::atualizarCidades);

    ui->rgPessoa->setCurrentIndex(0);
}

FormEmpresaDetalhe::~FormEmpresaDetalhe() {
    delete ui;
}

bool FormEmpresaDetalhe::validar() {
    bool isJuridica = ui->rgPessoa->currentIndex() == 1;
    QString cpfCnpj = isJuridica ? ui->edtCNPJ->text() : ui->edtCPF->text();
    QString razaoSocial = ui->edtRazaoSocial->text();

    cpfCnpj.remove('.').remove('-').remove('/');

    if (cpfCnpj.isEmpty()) {
        ToastNotification::show(this, "Aviso", Mensagens::Validacao::campoObrigatorio(ui->edtRazaoSocial->labelText()), ToastNotification::Warning, 3000);
        return false;
    }

    if (razaoSocial.isEmpty()) {
        ToastNotification::show(this, "Aviso", Mensagens::Validacao::campoObrigatorio(ui->edtCNPJ->labelText()), ToastNotification::Warning, 3000);
        return false;
    }

    return true;
}

void FormEmpresaDetalhe::carregarDados() {
    if (!m_idRegistro.has_value()) return;

    EmpresaRepository repo;
    m_empresaAtual = repo.buscarPorId(m_idRegistro.value());

    if (m_empresaAtual.has_value()) {
        const Empresa& emp = m_empresaAtual.value();

        bool isJuridica = emp.cnpjCpf().length() > 11;
        ui->rgPessoa->setCurrentIndex(isJuridica ? 1 : 0);

        if (isJuridica) ui->edtCNPJ->setText(emp.cnpjCpf());
        else ui->edtCPF->setText(emp.cnpjCpf());

        ui->edtRazaoSocial->setText(emp.razaoSocial());
        if (emp.nomeFantasia().has_value()) ui->edtNomeFantasia->setText(emp.nomeFantasia().value());

        // Find regime
        int indexRegime = ui->cbRegimeTributario->findIndex(emp.regimeTributario());
        if (indexRegime >= 0) ui->cbRegimeTributario->setCurrentIndex(indexRegime);

        // Endereco
        const Endereco& end = emp.endereco();
        ui->edtLogradouro->setText(end.logradouro());
        if (end.numero().has_value()) ui->edtNumero->setText(end.numero().value());
        if (end.complemento().has_value()) ui->edtComplemento->setText(end.complemento().value());
        ui->edtBairro->setText(end.bairro());
        ui->edtCEP->setText(end.cep());

        int indexUF = ui->cbUF->findIndex(end.cidade().idUF());
        if (indexUF >= 0) ui->cbUF->setCurrentIndex(indexUF);

        int indexCidade = ui->cbCidade->findIndex(end.idCidade());
        if (indexCidade >= 0) ui->cbCidade->setCurrentIndex(indexCidade);

        // For simplicity, we skip auto-selecting the UF and Cidade comboboxes here,
        // as CidadeRepository currently doesn't have buscarPorId() to easily find the State.
        // The user can re-select it, or you can implement buscarPorId in CidadeRepository later.
    }
}

void FormEmpresaDetalhe::onSalvar() {
    if (!validar()) return;

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

    cpfCnpj.remove('.').remove('-').remove('/');

    try {
        Empresa emp;
        Endereco endereco;

        if (m_empresaAtual.has_value()) {
            emp = m_empresaAtual.value();
            endereco = emp.endereco();
        }

        endereco.setLogradouro(enderecoLogradouro);
        endereco.setNumero(enderecoNumero.isEmpty() ? std::nullopt : std::make_optional(enderecoNumero));
        endereco.setComplemento(enderecoComplemento.isEmpty() ? std::nullopt : std::make_optional(enderecoComplemento));
        endereco.setBairro(enderecoBairro);
        endereco.setCep(enderecoCEP);
        endereco.setIdCidade(enderecoCidade);

        emp.setCnpjCpf(cpfCnpj);
        emp.setRazaoSocial(razaoSocial);
        emp.setNomeFantasia(nomeFantasia.isEmpty() ? std::nullopt : std::make_optional(nomeFantasia));
        emp.setRegimeTributario(regimeTributario);
        emp.setEndereco(endereco);

        EmpresaRepository repo;
        repo.salvar(emp);

        GlobalState::instance().setEmpresaAtual(emp);

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

void FormEmpresaDetalhe::onCnpjEditingFinished() {
    QString cnpj = ui->edtCNPJ->text();
    cnpj.remove('.').remove('-').remove('/');

    if (cnpj.length() == 14) {
        ICnpjService *service = m_cnpjService ? m_cnpjService : ICnpjService::create(this);
        if (!service) {
            ToastNotification::show(this, "Aviso", "Serviço de consulta de CNPJ não configurado.", ToastNotification::Warning, 3000);
            return;
        }

        connect(service, &ICnpjService::consultaFinalizada, this, &FormEmpresaDetalhe::onCnpjRetornado, Qt::UniqueConnection);
        connect(service, &ICnpjService::erroConsulta, this, &FormEmpresaDetalhe::onCnpjErro, Qt::UniqueConnection);

        ToastNotification::show(this, "Aguarde", "Consultando CNPJ...", ToastNotification::Info, 2000);
        service->consultarCNPJ(cnpj);
    }
}

void FormEmpresaDetalhe::onCnpjRetornado(const CnpjResult &resultado) {
    auto *service = qobject_cast<ICnpjService*>(sender());
    if (service && service != m_cnpjService) service->deleteLater();

    if (!resultado.isValid) {
        ToastNotification::show(this, "Aviso", "CNPJ inválido ou não encontrado na Receita.", ToastNotification::Warning, 3000);
        return;
    }

    if (!resultado.razaoSocial.isEmpty()) ui->edtRazaoSocial->setText(resultado.razaoSocial);
    if (!resultado.nomeFantasia.isEmpty()) ui->edtNomeFantasia->setText(resultado.nomeFantasia);

    if (!resultado.cep.isEmpty()) ui->edtCEP->setText(resultado.cep);
    if (!resultado.logradouro.isEmpty()) ui->edtLogradouro->setText(resultado.logradouro);
    if (!resultado.numero.isEmpty()) ui->edtNumero->setText(resultado.numero);
    if (!resultado.complemento.isEmpty()) ui->edtComplemento->setText(resultado.complemento);
    if (!resultado.bairro.isEmpty()) ui->edtBairro->setText(resultado.bairro);

    if (!resultado.siglaEstado.isEmpty()) {
        int indexUF = ui->cbUF->comboBox()->findText(resultado.siglaEstado);
        if (indexUF >= 0) ui->cbUF->setCurrentIndex(indexUF);
    }

    if (resultado.ibgeCidade > 0) {
        CidadeRepository cidadeRepo;
        auto cidadeOpt = cidadeRepo.buscarPorIbge(resultado.ibgeCidade);
        if (cidadeOpt.has_value()) {
            int indexCidade = ui->cbCidade->findIndex(cidadeOpt.value().id());
            if (indexCidade >= 0) ui->cbCidade->setCurrentIndex(indexCidade);
        }
    }

    ToastNotification::show(this, "Sucesso", "Dados preenchidos via Receita Federal!", ToastNotification::Success, 3000);
}

void FormEmpresaDetalhe::onCnpjErro(const QString &erro) {
    auto *service = qobject_cast<ICnpjService*>(sender());
    if (service && service != m_cnpjService) service->deleteLater();

    ToastNotification::show(this, "Erro", "Falha na consulta do CNPJ: " + erro, ToastNotification::Error, 3000);
}