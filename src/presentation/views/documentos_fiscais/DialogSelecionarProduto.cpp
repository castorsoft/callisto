#include "DialogSelecionarProduto.h"
#include "ui_DialogSelecionarProduto.h"

#include "../../../infrastructure/database/repositories/ProdutoRepository.h"
#include "../../../infrastructure/database/repositories/UnidadeMedidaRepository.h"

#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>

DialogSelecionarProduto::DialogSelecionarProduto(int idFornecedor,
                                                 const QString &nomeFornecedor,
                                                 const NotaTerceiroItem &itemInfo,
                                                 QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSelecionarProduto)
    , m_idFornecedor(idFornecedor)
    , m_itemInfo(itemInfo)
{
    ui->setupUi(this);

    // Preenche labels do item
    ui->lblFornecedor->setText(nomeFornecedor);
    ui->lblCodigoFornecedor->setText(itemInfo.codigoProduto());
    ui->lblDescricaoXml->setText(itemInfo.descricao());
    ui->lblCodigoBarrasXml->setText(itemInfo.codigoBarras().isEmpty() ? "Sem código de barras" : itemInfo.codigoBarras());
    ui->lblUnidadeXml->setText(itemInfo.unidadeMedida());

    ui->spinMultiplicador->setValue(itemInfo.multiplicador() > 0 ? itemInfo.multiplicador() : 1.0);

    configurarTabela();
    carregarUnidades();
    carregarProdutos();

    connect(ui->editBusca, &QLineEdit::textChanged, this, &DialogSelecionarProduto::onBuscaChanged);
    connect(ui->tableProdutos, &QTableWidget::itemSelectionChanged, this, &DialogSelecionarProduto::onTabelaSelectionChanged);
    connect(ui->tableProdutos, &QTableWidget::cellDoubleClicked, this, &DialogSelecionarProduto::onTabelaDoubleClicked);
    connect(ui->btnConfirmar, &InputButton::clicked, this, &DialogSelecionarProduto::onConfirmarClicked);
    connect(ui->btnCancelar, &InputButton::clicked, this, &QDialog::reject);
}

DialogSelecionarProduto::~DialogSelecionarProduto()
{
    delete ui;
}

void DialogSelecionarProduto::configurarTabela()
{
    ui->tableProdutos->setColumnCount(5);
    QStringList headers = {"ID", "Cód. Interno", "Descrição", "Cód. Barras", "Unidade"};
    ui->tableProdutos->setHorizontalHeaderLabels(headers);

    auto *header = ui->tableProdutos->horizontalHeader();
    header->setStretchLastSection(false);
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::Stretch);
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents);
}

void DialogSelecionarProduto::carregarUnidades()
{
    ui->comboUnidade->clear();
    UnidadeMedidaRepository umRepo;
    QList<UnidadeMedida> unidades = umRepo.listarTodas();

    int defaultIndex = -1;
    for (int i = 0; i < unidades.size(); ++i) {
        const auto &um = unidades[i];
        ui->comboUnidade->addItem(um.sigla(), um.id().value_or(0));

        if (m_itemInfo.idUnidadeMedidaInterna().has_value() && um.id() == m_itemInfo.idUnidadeMedidaInterna().value()) {
            defaultIndex = i;
        } else if (defaultIndex == -1 && um.sigla().compare(m_itemInfo.unidadeMedida(), Qt::CaseInsensitive) == 0) {
            defaultIndex = i;
        }
    }

    if (defaultIndex >= 0) {
        ui->comboUnidade->setCurrentIndex(defaultIndex);
    }
}

void DialogSelecionarProduto::carregarProdutos(const QString &filtro)
{
    ProdutoRepository prodRepo;
    QList<Produto> todos = prodRepo.listarTodos();

    m_produtosFiltrados.clear();
    QString term = filtro.trimmed().toLower();

    for (const auto &p : todos) {
        if (term.isEmpty()) {
            m_produtosFiltrados.append(p);
            continue;
        }

        bool matchDesc = p.descricao().toLower().contains(term);
        bool matchCod = p.codigoInterno().has_value() && p.codigoInterno().value().toLower().contains(term);
        bool matchEan = p.codigoBarra().has_value() && p.codigoBarra().value().toLower().contains(term);

        if (matchDesc || matchCod || matchEan) {
            m_produtosFiltrados.append(p);
        }
    }

    ui->tableProdutos->setRowCount(m_produtosFiltrados.size());

    int selectRow = -1;
    for (int i = 0; i < m_produtosFiltrados.size(); ++i) {
        const auto &p = m_produtosFiltrados[i];

        auto *itemId = new QTableWidgetItem(QString::number(p.id().value_or(0)));
        itemId->setTextAlignment(Qt::AlignCenter);

        auto *itemCod = new QTableWidgetItem(p.codigoInterno().value_or("-"));
        itemCod->setTextAlignment(Qt::AlignCenter);

        auto *itemDesc = new QTableWidgetItem(p.descricao());

        auto *itemEan = new QTableWidgetItem(p.codigoBarra().value_or("-"));
        itemEan->setTextAlignment(Qt::AlignCenter);

        auto *itemUn = new QTableWidgetItem(QString::number(p.idUnidadeMedida()));
        itemUn->setTextAlignment(Qt::AlignCenter);

        ui->tableProdutos->setItem(i, 0, itemId);
        ui->tableProdutos->setItem(i, 1, itemCod);
        ui->tableProdutos->setItem(i, 2, itemDesc);
        ui->tableProdutos->setItem(i, 3, itemEan);
        ui->tableProdutos->setItem(i, 4, itemUn);

        if (m_itemInfo.idProduto().has_value() && p.id() == m_itemInfo.idProduto().value()) {
            selectRow = i;
        }
    }

    if (selectRow >= 0) {
        ui->tableProdutos->selectRow(selectRow);
    } else {
        ui->btnConfirmar->setEnabled(false);
    }
}

void DialogSelecionarProduto::onBuscaChanged(const QString &texto)
{
    carregarProdutos(texto);
}

void DialogSelecionarProduto::onTabelaSelectionChanged()
{
    int row = ui->tableProdutos->currentRow();
    if (row >= 0 && row < m_produtosFiltrados.size()) {
        m_idProdutoSelecionado = m_produtosFiltrados[row].id().value_or(-1);
        ui->btnConfirmar->setEnabled(m_idProdutoSelecionado > 0);

        // Ajusta unidade se ainda não selecionada
        int idUm = m_produtosFiltrados[row].idUnidadeMedida();
        int idx = ui->comboUnidade->findData(idUm);
        if (idx >= 0) {
            ui->comboUnidade->setCurrentIndex(idx);
        }
    } else {
        m_idProdutoSelecionado = -1;
        ui->btnConfirmar->setEnabled(false);
    }
}

void DialogSelecionarProduto::onTabelaDoubleClicked(int /*row*/, int /*col*/)
{
    if (m_idProdutoSelecionado > 0) {
        onConfirmarClicked();
    }
}

void DialogSelecionarProduto::onConfirmarClicked()
{
    if (m_idProdutoSelecionado <= 0) {
        QMessageBox::warning(this, "Aviso", "Selecione um produto da lista para confirmar o vínculo.");
        return;
    }

    accept();
}

double DialogSelecionarProduto::multiplicador() const
{
    return ui->spinMultiplicador->value();
}

int DialogSelecionarProduto::idUnidadeMedida() const
{
    return ui->comboUnidade->currentData().toInt();
}
