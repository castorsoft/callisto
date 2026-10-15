#include "FormProdutoDetalhe.h"
#include "ui_FormProdutoDetalhe.h"
#include "../../components/ToastNotification.h"
#include "../../../core/entities/Produto.h"
#include "../../../infrastructure/database/repositories/ProdutoRepository.h"
#include "../../../infrastructure/database/repositories/NCMRepository.h"
#include "../../../infrastructure/database/repositories/UnidadeMedidaRepository.h"
#include "../../../core/GlobalState.h"
#include <QVariant>

FormProdutoDetalhe::FormProdutoDetalhe(QWidget *parent)
    : FormDetalheBase(parent)
    , ui(new Ui::FormProdutoDetalhe) {
    ui->setupUi(this);

    ui->edtTipoAtividade->addItem("00 – Mercadoria para Revenda", "00");
    ui->edtTipoAtividade->addItem("01 – Matéria-Prima", "01");
    ui->edtTipoAtividade->addItem("02 – Embalagem", "02");
    ui->edtTipoAtividade->addItem("03 – Produto em Processo", "03");
    ui->edtTipoAtividade->addItem("04 – Produto Acabado", "04");
    ui->edtTipoAtividade->addItem("05 – Subproduto", "05");
    ui->edtTipoAtividade->addItem("06 – Produto Intermediário", "06");
    ui->edtTipoAtividade->addItem("07 – Material de Uso e Consumo", "07");
    ui->edtTipoAtividade->addItem("08 – Ativo Imobilizado", "08");
    ui->edtTipoAtividade->addItem("09 – Serviços", "09");
    ui->edtTipoAtividade->addItem("10 – Outros insumos", "10");
    ui->edtTipoAtividade->addItem("99 – Outras", "99");

    connect(ui->lookupNCM, &InputLookup::searchRequested, this, &FormProdutoDetalhe::buscarNCM);
    connect(ui->lookupUnMedida, &InputLookup::searchRequested, this, &FormProdutoDetalhe::buscarUnMedida);

    // preencherUF();
    // atualizarCidades(1);

    // connect(ui->edtCNPJ, &InputMasked::editingFinished, this, &FormProdutoDetalhe::onCnpjEditingFinished);

    // connect(ui->cbFornecedor, &InputCheckBox::toggled, this, [this](bool) { atualizarVisibilidadeAbas(); });
    // connect(ui->cbContabilista, &InputCheckBox::toggled, this, [this](bool) { atualizarVisibilidadeAbas(); });

    // connect(ui->rgProduto, &InputRadioGroup::currentIndexChanged, this, &FormProdutoDetalhe::onProdutoChange);
    // connect(ui->btnCancelar, &QPushButton::clicked, this, &FormProdutoDetalhe::fechar);
    // connect(ui->btnSalvar, &QPushButton::clicked, this, &FormProdutoDetalhe::onSalvar);
    // connect(ui->cbUF, &InputComboBox::currentIndexChanged, this, &FormProdutoDetalhe::atualizarCidades);

    // ui->rgProduto->setCurrentIndex(0);
    // atualizarVisibilidadeAbas();
}

FormProdutoDetalhe::~FormProdutoDetalhe() {
    delete ui;
}

void FormProdutoDetalhe::buscarNCM(QString aTermo) {
    NCMRepository repo;

    auto lista = repo.buscarPorTermo(aTermo);

    QList<QPair<QString, QVariant>> resultados;
    for (const auto &ncm: lista) {
        QString textoExibicao = QString("%1 - %2").arg(ncm.codigo(), ncm.descricao());
        resultados.append({textoExibicao, ncm.id().value()});
    }
    ui->lookupNCM->setResults(resultados);
}

void FormProdutoDetalhe::buscarUnMedida(QString aTermo) {
    UnidadeMedidaRepository unRepo;

    auto lista = unRepo.buscarPorTermo(aTermo);

    QList<QPair<QString, QVariant>> resultados;
    for (const auto& unMedida: lista) {
        QString textoExibicao = QString("%1 - %2").arg(unMedida.sigla(), unMedida.descricao());
        resultados.append({textoExibicao, unMedida.id().value()});
    }
    ui->lookupUnMedida->setResults(resultados);
}

bool FormProdutoDetalhe::validar() {
    // bool isJuridica = ui->rgProduto->currentIndex() == 1;
    // QString cpfCnpj = isJuridica ? ui->edtCNPJ->text() : ui->edtCPF->text();
    // QString razaoSocial = ui->edtRazaoSocial->text();

    // cpfCnpj.remove('.').remove('-').remove('/');

    // if (cpfCnpj.isEmpty() || razaoSocial.isEmpty()) {
    //     ToastNotification::show(this, "Aviso", "Preencha os campos obrigatórios (CPF/CNPJ e Razão Social).", ToastNotification::Warning, 3000);
    //     return false;
    // }

    return true;
}

void FormProdutoDetalhe::carregarDados() {
    if (!m_idRegistro.has_value()) return;

    // ProdutoRepository repo;
    // m_ProdutoAtual = repo.buscarPorId(m_idRegistro.value());

    // if (m_ProdutoAtual.has_value()) {
    //     const Produto& p = m_ProdutoAtual.value();

    //     bool isJuridica = p.cnpjCpf().length() > 11;
    //     ui->rgProduto->setCurrentIndex(isJuridica ? 1 : 0);

    //     if (isJuridica) ui->edtCNPJ->setText(p.cnpjCpf());
    //     else ui->edtCPF->setText(p.cnpjCpf());

    //     ui->edtRazaoSocial->setText(p.razaoSocial());
    //     if (p.nomeFantasia().has_value()) ui->edtNomeFantasia->setText(p.nomeFantasia().value());
    //     if (p.inscricaoEstadual().has_value()) ui->edtInscricaoEstadual->setText(p.inscricaoEstadual().value());

    //     ui->cbFornecedor->setChecked(p.fornecedor() == "S");
    //     ui->cbContabilista->setChecked(p.contabilista() == "S");

    //     // Endereco
    //     const Endereco& end = p.endereco();
    //     ui->edtLogradouro->setText(end.logradouro());
    //     if (end.numero().has_value()) ui->edtNumero->setText(end.numero().value());
    //     if (end.complemento().has_value()) ui->edtComplemento->setText(end.complemento().value());
    //     ui->edtBairro->setText(end.bairro());
    //     ui->edtCEP->setText(end.cep());

    //     int indexUF = ui->cbUF->findIndex(end.cidade().idUF());
    //     if (indexUF >= 0) ui->cbUF->setCurrentIndex(indexUF);

    //     int indexCidade = ui->cbCidade->findIndex(end.idCidade());
    //     if (indexCidade >= 0) ui->cbCidade->setCurrentIndex(indexCidade);
    // }
}

void FormProdutoDetalhe::onSalvar() {
    if (!validar()) return;

    // bool isJuridica = ui->rgProduto->currentIndex() == 1;
    // QString cpfCnpj = isJuridica ? ui->edtCNPJ->text() : ui->edtCPF->text();
    // QString razaoSocial = ui->edtRazaoSocial->text();
    // QString nomeFantasia = ui->edtNomeFantasia->text();
    // QString inscricaoEstadual = ui->edtInscricaoEstadual->text();

    // QString fornecedor = ui->cbFornecedor->isChecked() ? "S" : "N";
    // QString contabilista = ui->cbContabilista->isChecked() ? "S" : "N";

    // QString enderecoLogradouro = ui->edtLogradouro->text();
    // QString enderecoNumero = ui->edtNumero->text();
    // QString enderecoComplemento = ui->edtComplemento->text();
    // QString enderecoBairro = ui->edtBairro->text();
    // QString enderecoCEP = ui->edtCEP->text();
    // int enderecoCidade = ui->cbCidade->currentData().toInt();

    // cpfCnpj.remove('.').remove('-').remove('/');

    // try {
    //     Produto p;
    //     Endereco endereco;

    //     if (m_ProdutoAtual.has_value()) {
    //         p = m_ProdutoAtual.value();
    //         endereco = p.endereco();
    //     }

    //     endereco.setLogradouro(enderecoLogradouro);
    //     endereco.setNumero(enderecoNumero.isEmpty() ? std::nullopt : std::make_optional(enderecoNumero));
    //     endereco.setComplemento(enderecoComplemento.isEmpty() ? std::nullopt : std::make_optional(enderecoComplemento));
    //     endereco.setBairro(enderecoBairro);
    //     endereco.setCep(enderecoCEP);
    //     endereco.setIdCidade(enderecoCidade);

    //     p.setCnpjCpf(cpfCnpj);
    //     p.setRazaoSocial(razaoSocial);
    //     p.setNomeFantasia(nomeFantasia.isEmpty() ? std::nullopt : std::make_optional(nomeFantasia));
    //     p.setInscricaoEstadual(inscricaoEstadual.isEmpty() ? std::nullopt : std::make_optional(inscricaoEstadual));

    //     p.setFornecedor(fornecedor);
    //     p.setContabilista(contabilista);
    //     p.setEndereco(endereco);

    //     p.setIdEmpresa(GlobalState::instance().empresaAtual()->id().value());

    //     ProdutoRepository repo;
    //     repo.salvar(p);

    //     ToastNotification::show(this, "Sucesso", "Produto salva com sucesso!", ToastNotification::Success, 3000);
    //     emit fechar();
    // } catch (const std::exception &e) {
    //     ToastNotification::show(this, "Erro", QString("Ocorreu um erro ao salvar a Produto:\n") + e.what(), ToastNotification::Error, 5000);
    // }
}