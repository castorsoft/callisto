#include "FormListagemBase.h"
#include "ui_FormListagemBase.h"
#include "providers/TableDataProviderAdapter.h"

#include <QSortFilterProxyModel>
#include <QRegularExpression>

class MultiColumnFilterProxyModel : public QSortFilterProxyModel
{
public:
    explicit MultiColumnFilterProxyModel(QObject *parent = nullptr)
        : QSortFilterProxyModel(parent)
    {
    }

    void setSearchTerm(const QString &termo)
    {
        QString normalizado = normalizarTexto(termo.trimmed());
        if (m_termoNormalizado == normalizado) {
            return;
        }
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        beginFilterChange();
#endif
        m_termoNormalizado = normalizado;
        m_termoLimpo = apenasAlfanumericos(normalizado);
#if QT_VERSION >= QT_VERSION_CHECK(6, 10, 0)
        endFilterChange(QSortFilterProxyModel::Direction::Rows);
#else
        invalidateRowsFilter();
#endif
    }

    static QString normalizarTexto(const QString &texto)
    {
        QString decomposto = texto.normalized(QString::NormalizationForm_D);
        QString resultado;
        resultado.reserve(decomposto.size());

        for (const QChar &ch : decomposto) {
            if (ch.category() != QChar::Mark_NonSpacing) {
                resultado.append(ch.toLower());
            }
        }
        return resultado;
    }

    static QString apenasAlfanumericos(const QString &texto)
    {
        QString resultado;
        resultado.reserve(texto.size());

        for (const QChar &ch : texto) {
            if (ch.isLetterOrNumber()) {
                resultado.append(ch);
            }
        }
        return resultado;
    }

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        if (m_termoNormalizado.isEmpty()) {
            return true;
        }

        QAbstractItemModel *src = sourceModel();
        if (!src) {
            return true;
        }

        const int colCount = src->columnCount(sourceParent);
        for (int col = 0; col < colCount; ++col) {
            QModelIndex idx = src->index(sourceRow, col, sourceParent);
            QString valor = src->data(idx, Qt::DisplayRole).toString();
            if (valor.isEmpty()) {
                continue;
            }

            QString valorNormalizado = normalizarTexto(valor);
            if (valorNormalizado.contains(m_termoNormalizado)) {
                return true;
            }

            if (!m_termoLimpo.isEmpty()) {
                QString valorLimpo = apenasAlfanumericos(valorNormalizado);
                if (valorLimpo.contains(m_termoLimpo)) {
                    return true;
                }
            }
        }

        return false;
    }

private:
    QString m_termoNormalizado;
    QString m_termoLimpo;
};

FormListagemBase::FormListagemBase(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormListagemBase)
{
    ui->setupUi(this);
    applyTableConfigurations();

    m_proxyModel = new MultiColumnFilterProxyModel(this);

    m_searchDebounceTimer = new QTimer(this);
    m_searchDebounceTimer->setSingleShot(true);
    m_searchDebounceTimer->setInterval(200);

    ui->lineEditSearch->setClearButtonEnabled(true);

    connect(ui->lineEditSearch, &QLineEdit::textChanged, this, &FormListagemBase::onSearchTextChanged);
    connect(ui->lineEditSearch, &QLineEdit::returnPressed, this, [this]() {
        m_searchDebounceTimer->stop();
        onPesquisar(ui->lineEditSearch->text());
    });
    connect(m_searchDebounceTimer, &QTimer::timeout, this, &FormListagemBase::onSearchTimerTimeout);
}

FormListagemBase::~FormListagemBase()
{
    delete ui;
}

void FormListagemBase::applyTableConfigurations()
{
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(false);
}

void FormListagemBase::setModel(QAbstractItemModel *model)
{
    if (m_model && m_model != model && m_model->parent() == this) {
        m_model->deleteLater();
    }

    m_model = model;
    m_proxyModel->setSourceModel(model);
    m_proxyModel->setSearchTerm(ui->lineEditSearch->text());
    ui->tableView->setModel(m_proxyModel);

    auto *header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(false);

    if (auto adapter = qobject_cast<TableDataProviderAdapter*>(model)) {
        if (auto provider = adapter->provider()) {
            for (int col = 0; col < provider->columnCount(); ++col) {
                auto resizeMode = provider->columnResizeMode(col);
                if (resizeMode.has_value()) {
                    header->setSectionResizeMode(col, resizeMode.value());
                } else {
                    QString headerText = provider->headerData(col).toLower();
                    if (headerText.contains("descri") || headerText.contains("razão") || headerText.contains("razao")) {
                        header->setSectionResizeMode(col, QHeaderView::Stretch);
                    } else if (provider->columnWidth(col).has_value()) {
                        header->setSectionResizeMode(col, QHeaderView::Interactive);
                    } else {
                        header->setSectionResizeMode(col, QHeaderView::ResizeToContents);
                    }
                }

                auto width = provider->columnWidth(col);
                if (width.has_value()) {
                    ui->tableView->setColumnWidth(col, width.value());
                }
            }
        }
    }
}

void FormListagemBase::onPesquisar(const QString &termo)
{
    if (m_proxyModel) {
        m_proxyModel->setSearchTerm(termo);
    }
}

QString FormListagemBase::termoPesquisa() const
{
    return ui->lineEditSearch->text().trimmed();
}

int FormListagemBase::sourceRowIndex(const QModelIndex &viewIndex) const
{
    if (!viewIndex.isValid()) {
        return -1;
    }

    if (m_proxyModel && viewIndex.model() == m_proxyModel) {
        return m_proxyModel->mapToSource(viewIndex).row();
    }

    return viewIndex.row();
}

void FormListagemBase::onSearchTextChanged(const QString &text)
{
    if (text.isEmpty()) {
        m_searchDebounceTimer->stop();
        onPesquisar(QString());
        return;
    }

    m_searchDebounceTimer->start();
}

void FormListagemBase::onSearchTimerTimeout()
{
    onPesquisar(ui->lineEditSearch->text());
}

void FormListagemBase::on_btnNew_clicked()
{
    emit actionNewRequested();
}

void FormListagemBase::on_btnEdit_clicked()
{
    if (!ui->tableView->selectionModel()) return;

    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        int sourceRow = sourceRowIndex(selection.first());
        if (sourceRow >= 0) {
            emit actionEditRequested(sourceRow);
        }
    }
}

void FormListagemBase::on_btnDelete_clicked()
{
    if (!ui->tableView->selectionModel()) return;

    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        int sourceRow = sourceRowIndex(selection.first());
        if (sourceRow >= 0) {
            emit actionDeleteRequested(sourceRow);
        }
    }
}
