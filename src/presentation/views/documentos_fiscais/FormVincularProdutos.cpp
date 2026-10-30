#include "FormVincularProdutos.h"
#include "ui_FormVincularProdutos.h"

#include "DialogSelecionarProduto.h"
#include "../produtos/FormProdutoDetalhe.h"
#include "../../components/ToastNotification.h"

#include "../../../infrastructure/database/repositories/DocumentoFiscalRepository.h"
#include "../../../infrastructure/database/repositories/ProdutoFornecedorRepository.h"

#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QColor>
#include <QBrush>
#include <QFont>

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

    connect(ui->tableNotas, &QTableWidget::itemSelectionChanged, this, &FormVincularProdutos::onNotaSelectionChanged);
    connect(ui->tableItens, &QTableWidget::itemSelectionChanged, this, &FormVincularProdutos::onItemSelectionChanged);

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
    // Tabela de Notas
    ui->tableNotas->setColumnCount(8);
    QStringList headersNotas = {"#", "NF-e / Série", "Emissão", "Fornecedor", "CNPJ / CPF", "Chave de Acesso", "Pendentes / Total", "Status"};
    ui->tableNotas->setHorizontalHeaderLabels(headersNotas);

    auto *hNotas = ui->tableNotas->horizontalHeader();
    hNotas->setStretchLastSection(false);
    hNotas->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    hNotas->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    hNotas->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    hNotas->setSectionResizeMode(3, QHeaderView::Stretch);
    hNotas->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    hNotas->setSectionResizeMode(5, QHeaderView::Interactive);
    hNotas->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    hNotas->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    ui->tableNotas->setColumnWidth(5, 220);

    // Tabela de Itens
    ui->tableItens->setColumnCount(8);
    QStringList headersItens = {"Item #", "Cód. Fornecedor", "Descrição (Fornecedor)", "Cód. Barras", "Unid. Forn.", "Status", "Produto Interno Vinculado", "Fator / Unidade"};
    ui->tableItens->setHorizontalHeaderLabels(headersItens);

    auto *hItens = ui->tableItens->horizontalHeader();
    hItens->setStretchLastSection(false);
    hItens->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    hItens->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    hItens->setSectionResizeMode(2, QHeaderView::Stretch);
    hItens->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    hItens->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    hItens->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    hItens->setSectionResizeMode(6, QHeaderView::Interactive);
    hItens->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    ui->tableItens->setColumnWidth(6, 260);
}

void FormVincularProdutos::carregarNotas()
{
    bool apenasPendentes = ui->comboFiltro->currentData().toBool();

    DocumentoFiscalRepository repo;
    m_todasNotas = repo.listarNotasTerceirosResumo(apenasPendentes);

    QString busca = ui->editBusca->text().trimmed().toLower();
    m_notasFiltradas.clear();

    for (const auto &n : m_todasNotas) {
        if (busca.isEmpty()) {
            m_notasFiltradas.append(n);
            continue;
        }

        bool matchForn = n.fornecedorNome().toLower().contains(busca);
        bool matchCnpj = n.fornecedorCnpj().contains(busca);
        bool matchChave = n.chaveAcesso().contains(busca);
        bool matchNum = QString::number(n.numero()).contains(busca);

        if (matchForn || matchCnpj || matchChave || matchNum) {
            m_notasFiltradas.append(n);
        }
    }

    ui->tableNotas->setRowCount(m_notasFiltradas.size());

    int selectIndex = -1;
    QFont fontBold;
    fontBold.setBold(true);

    for (int i = 0; i < m_notasFiltradas.size(); ++i) {
        const auto &n = m_notasFiltradas[i];

        auto *itemNum = new QTableWidgetItem(QString::number(i + 1));
        itemNum->setTextAlignment(Qt::AlignCenter);

        auto *itemNfe = new QTableWidgetItem(QString("%1 / %2").arg(n.numero()).arg(n.serie()));
        itemNfe->setTextAlignment(Qt::AlignCenter);

        auto *itemData = new QTableWidgetItem(n.dataEmissao().toString("dd/MM/yyyy HH:mm"));
        itemData->setTextAlignment(Qt::AlignCenter);

        auto *itemForn = new QTableWidgetItem(n.fornecedorNome());
        auto *itemCnpj = new QTableWidgetItem(n.fornecedorCnpj());
        itemCnpj->setTextAlignment(Qt::AlignCenter);

        auto *itemChave = new QTableWidgetItem(n.chaveAcesso());
        itemChave->setToolTip(n.chaveAcesso());

        auto *itemQtd = new QTableWidgetItem(QString("%1 / %2").arg(n.itensPendentes()).arg(n.totalItens()));
        itemQtd->setTextAlignment(Qt::AlignCenter);

        auto *itemStatus = new QTableWidgetItem();
        itemStatus->setTextAlignment(Qt::AlignCenter);
        itemStatus->setFont(fontBold);

        if (n.itensPendentes() > 0) {
            itemStatus->setText("Pendente");
            itemStatus->setForeground(QBrush(QColor("#E65100"))); // Laranja
        } else {
            itemStatus->setText("Concluído");
            itemStatus->setForeground(QBrush(QColor("#2E7D32"))); // Verde
        }

        ui->tableNotas->setItem(i, 0, itemNum);
        ui->tableNotas->setItem(i, 1, itemNfe);
        ui->tableNotas->setItem(i, 2, itemData);
        ui->tableNotas->setItem(i, 3, itemForn);
        ui->tableNotas->setItem(i, 4, itemCnpj);
        ui->tableNotas->setItem(i, 5, itemChave);
        ui->tableNotas->setItem(i, 6, itemQtd);
        ui->tableNotas->setItem(i, 7, itemStatus);

        if (n.idDocumentoFiscal() == m_idNotaSelecionada) {
            selectIndex = i;
        }
    }

    if (selectIndex >= 0) {
        ui->tableNotas->selectRow(selectIndex);
    } else if (!m_notasFiltradas.isEmpty()) {
        ui->tableNotas->selectRow(0);
    } else {
        m_idNotaSelecionada = -1;
        ui->tableItens->setRowCount(0);
        atualizarBotoesAcaoItem();
    }
}

void FormVincularProdutos::onFiltroChanged(int /*index*/)
{
    carregarNotas();
}

void FormVincularProdutos::onBuscaChanged(const QString &/*texto*/)
{
    carregarNotas();
}

void FormVincularProdutos::onRecarregarClicked()
{
    carregarNotas();
    ToastNotification::show(this, "Atualizado", "Lista de notas atualizada com sucesso.", ToastNotification::Info, 1500);
}

void FormVincularProdutos::onNotaSelectionChanged()
{
    int row = ui->tableNotas->currentRow();
    if (row >= 0 && row < m_notasFiltradas.size()) {
        const auto &nota = m_notasFiltradas[row];
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
        ui->tableItens->setRowCount(0);
        atualizarBotoesAcaoItem();
    }
}

void FormVincularProdutos::carregarItensDaNotaSelecionada()
{
    ui->tableItens->setRowCount(0);
    m_itensNotaAtual.clear();

    if (m_idNotaSelecionada <= 0) {
        atualizarBotoesAcaoItem();
        return;
    }

    DocumentoFiscalRepository repo;
    m_itensNotaAtual = repo.carregarItensNotaTerceiro(m_idNotaSelecionada);

    ui->tableItens->setRowCount(m_itensNotaAtual.size());

    QFont fontBold;
    fontBold.setBold(true);

    for (int i = 0; i < m_itensNotaAtual.size(); ++i) {
        const auto &item = m_itensNotaAtual[i];

        auto *itemNum = new QTableWidgetItem(QString::number(item.numeroItem()));
        itemNum->setTextAlignment(Qt::AlignCenter);

        auto *itemCodForn = new QTableWidgetItem(item.codigoProduto());
        itemCodForn->setTextAlignment(Qt::AlignCenter);

        auto *itemDescForn = new QTableWidgetItem(item.descricao());

        auto *itemBarras = new QTableWidgetItem(item.codigoBarras().isEmpty() ? "-" : item.codigoBarras());
        itemBarras->setTextAlignment(Qt::AlignCenter);

        auto *itemUnidForn = new QTableWidgetItem(item.unidadeMedida());
        itemUnidForn->setTextAlignment(Qt::AlignCenter);

        auto *itemStatus = new QTableWidgetItem();
        itemStatus->setTextAlignment(Qt::AlignCenter);
        itemStatus->setFont(fontBold);

        auto *itemProdInt = new QTableWidgetItem();
        auto *itemFator = new QTableWidgetItem();
        itemFator->setTextAlignment(Qt::AlignCenter);

        if (item.vinculado()) {
            itemStatus->setText("Vinculado");
            itemStatus->setForeground(QBrush(QColor("#2E7D32"))); // Verde

            QString descProd = item.produtoDescricao();
            if (!item.produtoCodigoInterno().isEmpty()) {
                descProd = QString("[%1] %2").arg(item.produtoCodigoInterno(), descProd);
            }
            itemProdInt->setText(descProd);

            itemFator->setText(QString("x%1 %2").arg(QString::number(item.multiplicador(), 'f', 2), item.siglaUnidadeMedidaInterna()));
        } else {
            itemStatus->setText("Pendente");
            itemStatus->setForeground(QBrush(QColor("#E65100"))); // Laranja

            itemProdInt->setText("Nenhum produto vinculado");
            itemProdInt->setForeground(QBrush(QColor("#757575")));

            itemFator->setText("-");
        }

        ui->tableItens->setItem(i, 0, itemNum);
        ui->tableItens->setItem(i, 1, itemCodForn);
        ui->tableItens->setItem(i, 2, itemDescForn);
        ui->tableItens->setItem(i, 3, itemBarras);
        ui->tableItens->setItem(i, 4, itemUnidForn);
        ui->tableItens->setItem(i, 5, itemStatus);
        ui->tableItens->setItem(i, 6, itemProdInt);
        ui->tableItens->setItem(i, 7, itemFator);
    }

    atualizarBotoesAcaoItem();
}

void FormVincularProdutos::onItemSelectionChanged()
{
    atualizarBotoesAcaoItem();
}

void FormVincularProdutos::atualizarBotoesAcaoItem()
{
    int row = ui->tableItens->currentRow();
    bool temItem = (row >= 0 && row < m_itensNotaAtual.size());

    if (!temItem) {
        ui->btnVincularExistente->setEnabled(false);
        ui->btnNovoProduto->setEnabled(false);
        ui->btnDesvincular->setEnabled(false);
        return;
    }

    const auto &item = m_itensNotaAtual[row];
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
    int row = ui->tableItens->currentRow();
    if (row < 0 || row >= m_itensNotaAtual.size()) return;

    const auto &item = m_itensNotaAtual[row];

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
    int row = ui->tableItens->currentRow();
    if (row < 0 || row >= m_itensNotaAtual.size()) return;

    const auto &item = m_itensNotaAtual[row];

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
    int row = ui->tableItens->currentRow();
    if (row < 0 || row >= m_itensNotaAtual.size()) return;

    const auto &item = m_itensNotaAtual[row];
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
