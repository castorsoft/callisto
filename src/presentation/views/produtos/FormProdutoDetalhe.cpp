#include "FormProdutoDetalhe.h"
#include "ui_FormProdutoDetalhe.h"
#include "../../components/ToastNotification.h"
#include "../../../core/entities/Produto.h"
#include "../../../infrastructure/database/repositories/ProdutoRepository.h"
#include "../../../infrastructure/database/repositories/NCMRepository.h"
#include "../../../infrastructure/database/repositories/UnidadeMedidaRepository.h"
#include "../../../core/GlobalState.h"
#include "../../../core/constants/Mensagens.h"
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

    connect(ui->btnCancelar, &InputButton::clicked, this, &FormProdutoDetalhe::fechar);
    connect(ui->btnSalvar, &InputButton::clicked, this, &FormProdutoDetalhe::onSalvar);

    // preencherUF();
    // atualizarCidades(1);

    // connect(ui->edtCNPJ, &InputMasked::editingFinished, this, &FormProdutoDetalhe::onCnpjEditingFinished);

    // connect(ui->cbFornecedor, &InputCheckBox::toggled, this, [this](bool) { atualizarVisibilidadeAbas(); });
    // connect(ui->cbContabilista, &InputCheckBox::toggled, this, [this](bool) { atualizarVisibilidadeAbas(); });

    // connect(ui->rgProduto, &InputRadioGroup::currentIndexChanged, this, &FormProdutoDetalhe::onProdutoChange);
    // connect(ui->btnCancelar, &InputButton::clicked, this, &FormProdutoDetalhe::fechar);
    // connect(ui->btnSalvar, &InputButton::clicked, this, &FormProdutoDetalhe::onSalvar);
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
        resultados.append({ncm.textoExibicao(), ncm.id().value()});
    }
    ui->lookupNCM->setResults(resultados);
}

void FormProdutoDetalhe::buscarUnMedida(QString aTermo) {
    UnidadeMedidaRepository unRepo;

    auto lista = unRepo.buscarPorTermo(aTermo);

    QList<QPair<QString, QVariant>> resultados;
    for (const auto& unMedida: lista) {
        resultados.append({unMedida.textoExibicao(), unMedida.id().value()});
    }
    ui->lookupUnMedida->setResults(resultados);
}

void FormProdutoDetalhe::preencherDadosIniciais(const QString &descricao,
                                               const QString &codigoBarras,
                                               const QString &siglaUnidade,
                                               const QString &codigoNcm)
{
    ui->edtDescricao->setText(descricao);

    if (!codigoBarras.isEmpty() && codigoBarras.compare("SEM GTIN", Qt::CaseInsensitive) != 0) {
        ui->edtCodBarras->setText(codigoBarras);
    }

    if (!siglaUnidade.isEmpty()) {
        UnidadeMedidaRepository unRepo;
        auto optUn = unRepo.buscarPorSigla(siglaUnidade);
        if (optUn.has_value()) {
            ui->lookupUnMedida->setCurrentData(optUn.value().id().value(), optUn.value().textoExibicao());
        }
    }

    if (!codigoNcm.isEmpty()) {
        NCMRepository ncmRepo;
        auto optNcm = ncmRepo.buscarPorCodigo(codigoNcm);
        if (optNcm.has_value()) {
            ui->lookupNCM->setCurrentData(optNcm.value().id().value(), optNcm.value().textoExibicao());
        }
    }

    if (ui->edtTipoAtividade->currentData().toString().isEmpty()) {
        ui->edtTipoAtividade->setCurrentIndex(0); // 00 - Mercadoria para Revenda
    }
}

bool FormProdutoDetalhe::validar() {
    QString descricao = ui->edtDescricao->text();
    int idUnidadeMedida = ui->lookupUnMedida->currentData().toInt();
    QString tipoAtividade = ui->edtTipoAtividade->currentData().toString();
    int idNCM = ui->lookupNCM->currentData().toInt();

    if (descricao.isEmpty()) {
        ToastNotification::show(this, "Aviso", Mensagens::Validacao::campoObrigatorio(ui->edtDescricao->labelText()), ToastNotification::Warning, 3000);
        return false;
    }

    if (idUnidadeMedida == 0) {
        ToastNotification::show(this, "Aviso", Mensagens::Validacao::campoObrigatorio(ui->lookupUnMedida->labelText()), ToastNotification::Warning, 3000);
        return false;
    }

    if (tipoAtividade.isEmpty()) {
        ToastNotification::show(this, "Aviso", Mensagens::Validacao::campoObrigatorio(ui->edtTipoAtividade->labelText()), ToastNotification::Warning, 3000);
        return false;
    }

    if (idNCM == 0) {
        ToastNotification::show(this, "Aviso", Mensagens::Validacao::campoObrigatorio(ui->lookupNCM->labelText()), ToastNotification::Warning, 3000);
        return false;
    }

    return true;
}

void FormProdutoDetalhe::carregarDados() {
    if (!m_idRegistro.has_value()) return;

    ProdutoRepository prodRepo;
    UnidadeMedidaRepository unRepo;
    NCMRepository ncmRepo;

    m_produtoAtual = prodRepo.buscarPorId(m_idRegistro.value());

    if (m_produtoAtual.has_value()) {
        const Produto& p = m_produtoAtual.value();

        ui->edtDescricao->setText(p.descricao());
        if (p.codigoBarra().has_value())
        ui->edtCodBarras->setText(p.codigoBarra().value());

        auto unMedida = unRepo.buscarPorId(p.idUnidadeMedida());
        if (unMedida.has_value()) {
            ui->lookupUnMedida->setCurrentData(p.idUnidadeMedida(), unMedida.value().textoExibicao());
        }

        auto ncm = ncmRepo.buscarPorId(p.idNCM());
        if (ncm.has_value()) {
            ui->lookupNCM->setCurrentData(p.idNCM(), ncm.value().textoExibicao());
        }
    }
}

void FormProdutoDetalhe::onSalvar() {
    if (!validar()) return;

    QString descricao = ui->edtDescricao->text();
    QString codBarras = ui->edtCodBarras->text();
    int idUnidadeMedida = ui->lookupUnMedida->currentData().toInt();
    QString tipoAtividade = ui->edtTipoAtividade->currentData().toString();
    int idNCM = ui->lookupNCM->currentData().toInt();


    try {
        Produto p;
        if (m_produtoAtual.has_value()) {
            p = m_produtoAtual.value();
        }

        p.setDescricao(descricao);
        p.setCodigoBarra(codBarras);
        p.setIdUnidadeMedida(idUnidadeMedida);
        p.setTipoAtividade(tipoAtividade);
        p.setIdNcm(idNCM);

        ProdutoRepository prodRepo;

        prodRepo.salvar(p);
        m_produtoAtual = p;
        m_idRegistro = p.id();
        m_salvo = true;
        ToastNotification::show(this, "Sucesso", Mensagens::Operacao::salvoSucesso("Produto"), ToastNotification::Success, 3000);
        emit produtoSalvo(p);
        emit fechar();
    } catch (const std::exception &e) {
        ToastNotification::show(this, "Erro", Mensagens::Operacao::erroSalvar("Produto", QString(e.what())), ToastNotification::Error, 5000);
    }
}