#include "FormPessoaDetalhe.h"
#include "ui_FormPessoaDetalhe.h"
#include "../../components/ToastNotification.h"
#include "../../../core/entities/Pessoa.h"
#include "../../../core/RepositoryProvider.h"
#include "../../../core/GlobalState.h"
#include "../../../core/constants/Mensagens.h"
#include <QVariant>

FormPessoaDetalhe::FormPessoaDetalhe(QWidget *parent, ICnpjService *cnpjService)
    : FormDetalheBase(parent)
    , ui(new Ui::FormPessoaDetalhe)
    , m_cnpjService(cnpjService) {
    ui->setupUi(this);

    ui->rgPessoa->addOption("Física");
    ui->rgPessoa->addOption("Jurídica");

    preencherUF();
    atualizarCidades(1);

    connect(ui->edtCNPJ, &InputMasked::editingFinished, this, &FormPessoaDetalhe::onCnpjEditingFinished);

    connect(ui->cbFornecedor, &InputCheckBox::toggled, this, [this](bool) { atualizarVisibilidadeAbas(); });
    connect(ui->cbContabilista, &InputCheckBox::toggled, this, [this](bool) { atualizarVisibilidadeAbas(); });

    connect(ui->rgPessoa, &InputRadioGroup::currentIndexChanged, this, &FormPessoaDetalhe::onPessoaChange);
    connect(ui->btnCancelar, &InputButton::clicked, this, &FormPessoaDetalhe::fechar);
    connect(ui->btnSalvar, &InputButton::clicked, this, &FormPessoaDetalhe::onSalvar);
    connect(ui->cbUF, &InputComboBox::currentIndexChanged, this, &FormPessoaDetalhe::atualizarCidades);

    ui->rgPessoa->setCurrentIndex(0);
    atualizarVisibilidadeAbas();
}

FormPessoaDetalhe::~FormPessoaDetalhe() {
    delete ui;
}

bool FormPessoaDetalhe::validar() {
    bool isJuridica = ui->rgPessoa->currentIndex() == 1;
    QString cpfCnpj = isJuridica ? ui->edtCNPJ->text() : ui->edtCPF->text();
    QString razaoSocial = ui->edtRazaoSocial->text();

    cpfCnpj.remove('.').remove('-').remove('/');

    if (cpfCnpj.isEmpty()) {
        ToastNotification::show(this, "Aviso", Mensagens::Validacao::campoInvalido("CNPJ/CPF"), ToastNotification::Warning, 3000);
        return false;
    }

    if (razaoSocial.isEmpty()) {
        ToastNotification::show(this, "Aviso", Mensagens::Validacao::campoInvalido(ui->edtRazaoSocial->labelText()), ToastNotification::Warning, 3000);
        return false;
    }

    return true;
}

void FormPessoaDetalhe::carregarDados() {
    if (!m_idRegistro.has_value()) return;

    auto &repo = RepositoryProvider::pessoaRepository();
    m_PessoaAtual = repo.buscarPorId(m_idRegistro.value());

    if (m_PessoaAtual.has_value()) {
        const Pessoa& p = m_PessoaAtual.value();

        bool isJuridica = p.cnpjCpf().length() > 11;
        ui->rgPessoa->setCurrentIndex(isJuridica ? 1 : 0);

        if (isJuridica) ui->edtCNPJ->setText(p.cnpjCpf());
        else ui->edtCPF->setText(p.cnpjCpf());

        ui->edtRazaoSocial->setText(p.razaoSocial());
        if (p.nomeFantasia().has_value()) ui->edtNomeFantasia->setText(p.nomeFantasia().value());
        if (p.inscricaoEstadual().has_value()) ui->edtInscricaoEstadual->setText(p.inscricaoEstadual().value());

        ui->cbFornecedor->setChecked(p.fornecedor() == "S");
        ui->cbContabilista->setChecked(p.contabilista() == "S");

        if (p.pessoaContabilista().has_value()) {
            const PessoaContabilista &pc = p.pessoaContabilista().value();
            ui->edtCRCContabilista->setText(pc.crc());
            ui->edtCPFContabilista->setText(pc.cpf());
        } else {
            ui->edtCRCContabilista->setText("");
            ui->edtCPFContabilista->setText("");
        }

        // Endereco
        const Endereco& end = p.endereco();
        ui->edtLogradouro->setText(end.logradouro());
        if (end.numero().has_value()) ui->edtNumero->setText(end.numero().value());
        if (end.complemento().has_value()) ui->edtComplemento->setText(end.complemento().value());
        ui->edtBairro->setText(end.bairro());
        ui->edtCEP->setText(end.cep());

        int indexUF = ui->cbUF->findIndex(end.cidade().idUF());
        if (indexUF >= 0) ui->cbUF->setCurrentIndex(indexUF);

        int indexCidade = ui->cbCidade->findIndex(end.idCidade());
        if (indexCidade >= 0) ui->cbCidade->setCurrentIndex(indexCidade);
    }
}

void FormPessoaDetalhe::onSalvar() {
    if (!validar()) return;

    bool isJuridica = ui->rgPessoa->currentIndex() == 1;
    QString cpfCnpj = isJuridica ? ui->edtCNPJ->text() : ui->edtCPF->text();
    QString razaoSocial = ui->edtRazaoSocial->text();
    QString nomeFantasia = ui->edtNomeFantasia->text();
    QString inscricaoEstadual = ui->edtInscricaoEstadual->text();

    QString fornecedor = ui->cbFornecedor->isChecked() ? "S" : "N";
    QString contabilista = ui->cbContabilista->isChecked() ? "S" : "N";

    QString enderecoLogradouro = ui->edtLogradouro->text();
    QString enderecoNumero = ui->edtNumero->text();
    QString enderecoComplemento = ui->edtComplemento->text();
    QString enderecoBairro = ui->edtBairro->text();
    QString enderecoCEP = ui->edtCEP->text();
    int enderecoCidade = ui->cbCidade->currentData().toInt();

    cpfCnpj.remove('.').remove('-').remove('/');

    try {
        Pessoa p;
        Endereco endereco;

        if (m_PessoaAtual.has_value()) {
            p = m_PessoaAtual.value();
            endereco = p.endereco();
        }

        endereco.setLogradouro(enderecoLogradouro);
        endereco.setNumero(enderecoNumero.isEmpty() ? std::nullopt : std::make_optional(enderecoNumero));
        endereco.setComplemento(enderecoComplemento.isEmpty() ? std::nullopt : std::make_optional(enderecoComplemento));
        endereco.setBairro(enderecoBairro);
        endereco.setCep(enderecoCEP);
        endereco.setIdCidade(enderecoCidade);

        p.setCnpjCpf(cpfCnpj);
        p.setRazaoSocial(razaoSocial);
        p.setNomeFantasia(nomeFantasia.isEmpty() ? std::nullopt : std::make_optional(nomeFantasia));
        p.setInscricaoEstadual(inscricaoEstadual.isEmpty() ? std::nullopt : std::make_optional(inscricaoEstadual));

        p.setFornecedor(fornecedor);
        p.setContabilista(contabilista);
        p.setEndereco(endereco);

        if (contabilista == "S") {
            PessoaContabilista pc = p.pessoaContabilista().value_or(PessoaContabilista());
            QString cpfCont = ui->edtCPFContabilista->text().trimmed();
            cpfCont.remove('.').remove('-').remove('/');
            pc.setCrc(ui->edtCRCContabilista->text().trimmed());
            pc.setCpf(cpfCont);
            p.setPessoaContabilista(pc);
        } else {
            p.setPessoaContabilista(std::nullopt);
        }

        p.setIdEmpresa(GlobalState::instance().empresaAtual()->id().value());

        auto &repo = RepositoryProvider::pessoaRepository();
        repo.salvar(p);

        ToastNotification::show(this, "Sucesso", "Pessoa salva com sucesso!", ToastNotification::Success, 3000);
        emit fechar();
    } catch (const std::exception &e) {
        ToastNotification::show(this, "Erro", QString("Ocorreu um erro ao salvar a Pessoa:\n") + e.what(), ToastNotification::Error, 5000);
    }
}

void FormPessoaDetalhe::preencherUF() {
    ui->cbUF->clear();
    auto &repo = RepositoryProvider::estadoRepository();
    QList<Estado> estados = repo.listarTodos();
    for (const Estado& estado : estados) {
        ui->cbUF->addItem(estado.sigla(), estado.id());
    }
}

void FormPessoaDetalhe::atualizarCidades(int indexUF) {
    ui->cbCidade->clear();

    QVariant ufData = ui->cbUF->comboBox()->itemData(indexUF);
    if (!ufData.isValid()) return;

    int idEstado = ufData.toInt();

    auto &repo = RepositoryProvider::cidadeRepository();
    QList<Cidade> cidades = repo.listarPorEstado(idEstado);
    for (const Cidade& cidade : cidades) {
        ui->cbCidade->addItem(cidade.nome(), cidade.id());
    }
}

void FormPessoaDetalhe::onPessoaChange(int index) {
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

void FormPessoaDetalhe::onCnpjEditingFinished() {
    QString cnpj = ui->edtCNPJ->text();
    cnpj.remove('.').remove('-').remove('/');

    if (cnpj.length() == 14) {
        ICnpjService *service = m_cnpjService ? m_cnpjService : ICnpjService::create(this);
        if (!service) {
            ToastNotification::show(this, "Aviso", "Serviço de consulta de CNPJ não configurado.", ToastNotification::Warning, 3000);
            return;
        }

        connect(service, &ICnpjService::consultaFinalizada, this, &FormPessoaDetalhe::onCnpjRetornado, Qt::UniqueConnection);
        connect(service, &ICnpjService::erroConsulta, this, &FormPessoaDetalhe::onCnpjErro, Qt::UniqueConnection);

        ToastNotification::show(this, "Aguarde", "Consultando CNPJ...", ToastNotification::Info, 2000);
        service->consultarCNPJ(cnpj);
    }
}

void FormPessoaDetalhe::onCnpjRetornado(const CnpjResult &resultado) {
    auto *service = qobject_cast<ICnpjService*>(sender());
    if (service && service != m_cnpjService) service->deleteLater();

    if (!resultado.isValid) {
        ToastNotification::show(this, "Aviso", "CNPJ inválido ou não encontrado na Receita.", ToastNotification::Warning, 3000);
        return;
    }

    if (!resultado.razaoSocial.isEmpty()) ui->edtRazaoSocial->setText(resultado.razaoSocial);
    if (!resultado.nomeFantasia.isEmpty()) ui->edtNomeFantasia->setText(resultado.nomeFantasia);
    if (!resultado.inscricaoEstadual.isEmpty()) ui->edtInscricaoEstadual->setText(resultado.inscricaoEstadual);

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
        auto &cidadeRepo = RepositoryProvider::cidadeRepository();
        auto cidadeOpt = cidadeRepo.buscarPorIbge(resultado.ibgeCidade);
        if (cidadeOpt.has_value()) {
            int indexCidade = ui->cbCidade->findIndex(cidadeOpt.value().id());
            if (indexCidade >= 0) ui->cbCidade->setCurrentIndex(indexCidade);
        }
    }

    ToastNotification::show(this, "Sucesso", "Dados preenchidos via Receita Federal!", ToastNotification::Success, 3000);
}

void FormPessoaDetalhe::onCnpjErro(const QString &erro) {
    auto *service = qobject_cast<ICnpjService*>(sender());
    if (service && service != m_cnpjService) service->deleteLater();

    ToastNotification::show(this, "Erro", "Falha na consulta do CNPJ: " + erro, ToastNotification::Error, 3000);
}

void FormPessoaDetalhe::atualizarVisibilidadeAbas() {
    int idxF = ui->tabWidget->indexOf(ui->tabFornecedor);
    if (idxF != -1) ui->tabWidget->setTabVisible(idxF, ui->cbFornecedor->isChecked());

    int idxC = ui->tabWidget->indexOf(ui->tabContabilista);
    if (idxC != -1) ui->tabWidget->setTabVisible(idxC, ui->cbContabilista->isChecked());
}