#include "FormVincularProdutos.h"
#include "ui_FormVincularProdutos.h"

#include "DialogSelecionarProduto.h"
#include "../produtos/FormProdutoDetalhe.h"
#include "../../components/ToastNotification.h"

#include "../../../infrastructure/database/repositories/DocumentoFiscalRepository.h"
#include "../../../infrastructure/database/repositories/ProdutoFornecedorRepository.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QSortFilterProxyModel>

namespace {

class NotasFilterProxyModel : public QSortFilterProxyModel {
public:
    explicit NotasFilterProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent)
    {
    }

    void setSearchTerm(const QString &term)
    {
        QString trimmed = term.trimmed();
        if (m_term == trimmed) return;
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        beginFilterChange();
#endif
        m_term = trimmed;
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
        invalidateRowsFilter();
#endif
    }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        if (m_term.isEmpty()) {
            return true;
        }

        QAbstractItemModel *src = sourceModel();
        if (!src) {
            return true;
        }

        const int colCount = src->columnCount(sourceParent);
        for (int col = 0; col < colCount; ++col) {
            QModelIndex idx = src->index(sourceRow, col, sourceParent);
            QString val = src->data(idx, Qt::DisplayRole).toString();
            if (val.contains(m_term, Qt::CaseInsensitive)) {
                return true;
            }
        }

        return false;
    }

private:
    QString m_term;
};

} // namespace

FormVincularProdutos::FormVincularProdutos(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormVincularProdutos)
{
    ui->setupUi(this);

    ui->comboFiltro->addItem("Apenas notas com itens pendentes", true);
    ui->comboFiltro->addItem("Todas as notas de terceiros", false);

    configurarTabelas();

    connect(ui->comboFiltro, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &FormVincularProdutos::onFiltroChanged);
    connect(ui->editBusca, &QLineEdit::textChanged, this, &FormVincularProdutos::onBuscaChanged);
    connect(ui->btnRecarregar, &InputButton::clicked, this, &FormVincularProdutos::onRecarregarClicked);

    connect(ui->btnVincularExistente, &InputButton::clicked, this, &FormVincularProdutos::onVincularExistenteClicked);
    connect(ui->btnNovoProduto, &InputButton::clicked, this, &FormVincularProdutos::onNovoProdutoClicked);
    connect(ui->btnDesvincular, &InputButton::clicked, this, &FormVincularProdutos::onDesvincularClicked);

    // Ajusta proporção inicial do splitter (40% topo, 60% base)
    ui->splitter->setSizes({250, 350});

    carregarNotas();
}

FormVincularProdutos::~FormVincularProdutos()
{
    delete ui;
}

void FormVincularProdutos::configurarTabelas()
{
    ui->tableNotas->setAlternatingRowColors(true);
    ui->tableNotas->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableNotas->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableNotas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableNotas->horizontalHeader()->setStretchLastSection(false);

    ui->tableItens->setAlternatingRowColors(true);
    ui->tableItens->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableItens->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableItens->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableItens->horizontalHeader()->setStretchLastSection(false);
}

void FormVincularProdutos::carregarNotas()
{
    bool apenasPendentes = ui->comboFiltro->currentData().toBool();

    DocumentoFiscalRepository repo;
    auto notas = repo.listarNotasTerceirosResumo(apenasPendentes);

    m_notasProvider = std::make_shared<NotaTerceiroListProvider>(notas);
    m_notasAdapter = new TableDataProviderAdapter(m_notasProvider, this);

    if (!m_notasProxy) {
        m_notasProxy = new NotasFilterProxyModel(this);
    }
    m_notasProxy->setSourceModel(m_notasAdapter);
    static_cast<NotasFilterProxyModel *>(m_notasProxy)->setSearchTerm(ui->editBusca->text());

    ui->tableNotas->setModel(m_notasProxy);
    aplicarConfiguracoesTabelaNotas();

    if (ui->tableNotas->selectionModel()) {
        connect(ui->tableNotas->selectionModel(), &QItemSelectionModel::currentRowChanged,
                this, &FormVincularProdutos::onNotaSelectionChanged, Qt::UniqueConnection);
    }

    int selectProxyRow = -1;
    if (m_idNotaSelecionada > 0 && m_notasProvider) {
        int srcRow = m_notasProvider->findRowById(m_idNotaSelecionada);
        if (srcRow >= 0 && m_notasProxy && m_notasAdapter) {
            QModelIndex srcIdx = m_notasAdapter->index(srcRow, 0);
            QModelIndex proxyIdx = m_notasProxy->mapFromSource(srcIdx);
            if (proxyIdx.isValid()) {
                selectProxyRow = proxyIdx.row();
            }
        }
    }

    if (selectProxyRow >= 0) {
        ui->tableNotas->selectRow(selectProxyRow);
    } else if (m_notasProxy->rowCount() > 0) {
        ui->tableNotas->selectRow(0);
    } else {
        m_idNotaSelecionada = -1;
        m_idFornecedorNotaAtual = -1;
        m_nomeFornecedorNotaAtual.clear();
        ui->groupItens->setTitle("Itens da Nota Selecionada");

        m_itensProvider = std::make_shared<NotaTerceiroItemListProvider>(QList<NotaTerceiroItem>{});
        m_itensAdapter = new TableDataProviderAdapter(m_itensProvider, this);
        ui->tableItens->setModel(m_itensAdapter);
        aplicarConfiguracoesTabelaItens();

        atualizarBotoesAcaoItem();
    }
}

void FormVincularProdutos::aplicarConfiguracoesTabelaNotas()
{
    if (!m_notasProvider) return;

    auto *header = ui->tableNotas->horizontalHeader();
    header->setStretchLastSection(false);

    for (int col = 0; col < m_notasProvider->columnCount(); ++col) {
        auto resizeMode = m_notasProvider->columnResizeMode(col);
        if (resizeMode.has_value()) {
            header->setSectionResizeMode(col, resizeMode.value());
        } else {
            header->setSectionResizeMode(col, QHeaderView::ResizeToContents);
        }

        auto width = m_notasProvider->columnWidth(col);
        if (width.has_value()) {
            ui->tableNotas->setColumnWidth(col, width.value());
        }
    }
}

void FormVincularProdutos::aplicarConfiguracoesTabelaItens()
{
    if (!m_itensProvider) return;

    auto *header = ui->tableItens->horizontalHeader();
    header->setStretchLastSection(false);

    for (int col = 0; col < m_itensProvider->columnCount(); ++col) {
        auto resizeMode = m_itensProvider->columnResizeMode(col);
        if (resizeMode.has_value()) {
            header->setSectionResizeMode(col, resizeMode.value());
        } else {
            header->setSectionResizeMode(col, QHeaderView::ResizeToContents);
        }

        auto width = m_itensProvider->columnWidth(col);
        if (width.has_value()) {
            ui->tableItens->setColumnWidth(col, width.value());
        }
    }
}

std::optional<NotaTerceiroResumo> FormVincularProdutos::notaSelecionada() const
{
    if (!m_notasProvider || !m_notasProxy) return std::nullopt;

    QModelIndex current = ui->tableNotas->currentIndex();
    if (!current.isValid()) return std::nullopt;

    int srcRow = m_notasProxy->mapToSource(current).row();
    return m_notasProvider->getNotaAt(srcRow);
}

std::optional<NotaTerceiroItem> FormVincularProdutos::itemSelecionado() const
{
    if (!m_itensProvider) return std::nullopt;

    QModelIndex current = ui->tableItens->currentIndex();
    if (!current.isValid()) return std::nullopt;

    return m_itensProvider->getItemAt(current.row());
}

void FormVincularProdutos::onFiltroChanged(int /*index*/)
{
    carregarNotas();
}

void FormVincularProdutos::onBuscaChanged(const QString &texto)
{
    if (m_notasProxy) {
        static_cast<NotasFilterProxyModel *>(m_notasProxy)->setSearchTerm(texto);
        if (ui->tableNotas->currentIndex().isValid()) {
            onNotaSelectionChanged();
        } else if (m_notasProxy->rowCount() > 0) {
            ui->tableNotas->selectRow(0);
        } else {
            onNotaSelectionChanged();
        }
    }
}

void FormVincularProdutos::onRecarregarClicked()
{
    carregarNotas();
}

void FormVincularProdutos::onNotaSelectionChanged()
{
    auto optNota = notaSelecionada();
    if (optNota.has_value()) {
        const auto &nota = optNota.value();
        m_idNotaSelecionada = nota.idDocumentoFiscal();
        m_idFornecedorNotaAtual = nota.idFornecedor();
        m_nomeFornecedorNotaAtual = nota.fornecedorNome();

        ui->groupItens->setTitle(QString("Itens da NF-e %1 (Fornecedor: %2)").arg(nota.numero()).arg(nota.fornecedorNome()));
        carregarItensDaNotaSelecionada();
    } else {
        m_idNotaSelecionada = -1;
        m_idFornecedorNotaAtual = -1;
        m_nomeFornecedorNotaAtual.clear();
        ui->groupItens->setTitle("Itens da Nota Selecionada");

        m_itensProvider = std::make_shared<NotaTerceiroItemListProvider>(QList<NotaTerceiroItem>{});
        m_itensAdapter = new TableDataProviderAdapter(m_itensProvider, this);
        ui->tableItens->setModel(m_itensAdapter);
        aplicarConfiguracoesTabelaItens();

        atualizarBotoesAcaoItem();
    }
}

void FormVincularProdutos::carregarItensDaNotaSelecionada()
{
    QList<NotaTerceiroItem> itens;
    if (m_idNotaSelecionada > 0) {
        DocumentoFiscalRepository repo;
        itens = repo.carregarItensNotaTerceiro(m_idNotaSelecionada);
    }

    m_itensProvider = std::make_shared<NotaTerceiroItemListProvider>(itens);
    m_itensAdapter = new TableDataProviderAdapter(m_itensProvider, this);
    ui->tableItens->setModel(m_itensAdapter);
    aplicarConfiguracoesTabelaItens();

    if (ui->tableItens->selectionModel()) {
        connect(ui->tableItens->selectionModel(), &QItemSelectionModel::currentRowChanged,
                this, &FormVincularProdutos::onItemSelectionChanged, Qt::UniqueConnection);
    }

    if (m_itensProvider->rowCount() > 0) {
        ui->tableItens->selectRow(0);
    } else {
        atualizarBotoesAcaoItem();
    }
}

void FormVincularProdutos::onItemSelectionChanged()
{
    atualizarBotoesAcaoItem();
}

void FormVincularProdutos::atualizarBotoesAcaoItem()
{
    auto optItem = itemSelecionado();
    if (!optItem.has_value()) {
        ui->btnVincularExistente->setEnabled(false);
        ui->btnNovoProduto->setEnabled(false);
        ui->btnDesvincular->setEnabled(false);
        return;
    }

    const auto &item = optItem.value();
    ui->btnVincularExistente->setEnabled(true);
    ui->btnNovoProduto->setEnabled(true);
    ui->btnDesvincular->setEnabled(item.vinculado());

    if (item.vinculado()) {
        ui->btnVincularExistente->setText("Alterar Vínculo...");
    } else {
        ui->btnVincularExistente->setText("Vincular a Produto Existente...");
    }
}

void FormVincularProdutos::onVincularExistenteClicked()
{
    auto optItem = itemSelecionado();
    if (!optItem.has_value()) return;

    const auto &item = optItem.value();

    DialogSelecionarProduto dlg(m_idFornecedorNotaAtual, m_nomeFornecedorNotaAtual, item, this);
    if (dlg.exec() == QDialog::Accepted) {
        int idProd = dlg.idProdutoSelecionado();
        double mult = dlg.multiplicador();
        int idUm = dlg.idUnidadeMedida();

        ProdutoFornecedor pf(idProd, m_idFornecedorNotaAtual, item.codigoProduto(), idUm, mult, std::nullopt);
        ProdutoFornecedorRepository pfRepo;

        if (pfRepo.salvar(pf)) {
            ToastNotification::show(this, "Sucesso", "Vínculo salvo com sucesso!", ToastNotification::Success, 3000);
            carregarItensDaNotaSelecionada();
            carregarNotas();
        } else {
            QMessageBox::critical(this, "Erro", "Não foi possível gravar o vínculo no banco de dados.");
        }
    }
}

void FormVincularProdutos::onNovoProdutoClicked()
{
    auto optItem = itemSelecionado();
    if (!optItem.has_value()) return;

    const auto &item = optItem.value();

    QDialog dlg(this);
    dlg.setWindowTitle("Cadastrar Novo Produto");
    dlg.resize(600, 520);

    auto *layout = new QVBoxLayout(&dlg);
    layout->setContentsMargins(10, 10, 10, 10);

    auto *formDetalhe = new FormProdutoDetalhe(&dlg);
    formDetalhe->preencherDadosIniciais(item.descricao(), item.codigoBarras(), item.unidadeMedida(), item.ncm());
    layout->addWidget(formDetalhe);

    connect(formDetalhe, &FormDetalheBase::fechar, &dlg, [&dlg, formDetalhe]() {
        if (formDetalhe->salvo()) {
            dlg.accept();
        } else {
            dlg.reject();
        }
    });

    if (dlg.exec() == QDialog::Accepted && formDetalhe->salvo()) {
        auto optProd = formDetalhe->produtoAtual();
        if (!optProd.has_value() || !optProd.value().id().has_value()) {
            return;
        }

        int idNovoProd = optProd.value().id().value();
        int idUm = optProd.value().idUnidadeMedida();
        double mult = 1.0;

        ProdutoFornecedor pf(idNovoProd, m_idFornecedorNotaAtual, item.codigoProduto(), idUm, mult, std::nullopt);
        ProdutoFornecedorRepository pfRepo;

        if (pfRepo.salvar(pf)) {
            ToastNotification::show(this, "Sucesso", "Novo produto cadastrado e vinculado com sucesso!", ToastNotification::Success, 3500);
            carregarItensDaNotaSelecionada();
            carregarNotas();
        } else {
            QMessageBox::critical(this, "Erro", "Produto cadastrado, mas falhou ao registrar o vínculo com o fornecedor.");
        }
    }
}

void FormVincularProdutos::onDesvincularClicked()
{
    auto optItem = itemSelecionado();
    if (!optItem.has_value()) return;

    const auto &item = optItem.value();
    if (!item.vinculado() || !item.idProdutoFornecedor().has_value()) return;

    auto resp = QMessageBox::question(
        this,
        "Confirmar Desvinculação",
        QString("Deseja remover a associação do item \"%1\" (Cód: %2) com o produto \"%3\"?")
            .arg(item.descricao(), item.codigoProduto(), item.produtoDescricao()),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (resp == QMessageBox::Yes) {
        ProdutoFornecedorRepository pfRepo;
        if (pfRepo.deletar(item.idProdutoFornecedor().value())) {
            ToastNotification::show(this, "Desvinculado", "Vínculo removido com sucesso.", ToastNotification::Info, 2500);
            carregarItensDaNotaSelecionada();
            carregarNotas();
        } else {
            QMessageBox::critical(this, "Erro", "Não foi possível remover o vínculo.");
        }
    }
}
