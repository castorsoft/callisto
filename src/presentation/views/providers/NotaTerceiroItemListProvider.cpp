#include "NotaTerceiroItemListProvider.h"

#include <QColor>
#include <QBrush>
#include <QFont>

NotaTerceiroItemListProvider::NotaTerceiroItemListProvider(const QList<NotaTerceiroItem> &itens)
    : m_itens(itens)
{
    m_headers << "Item #"
              << "Cód. Fornecedor"
              << "Descrição (Fornecedor)"
              << "Cód. Barras"
              << "Unid. Forn."
              << "Status"
              << "Produto Interno Vinculado"
              << "Fator / Unidade";
}

int NotaTerceiroItemListProvider::rowCount() const
{
    return m_itens.size();
}

int NotaTerceiroItemListProvider::columnCount() const
{
    return m_headers.size();
}

QVariant NotaTerceiroItemListProvider::data(int row, int column) const
{
    return data(row, column, Qt::DisplayRole);
}

QVariant NotaTerceiroItemListProvider::data(int row, int column, int role) const
{
    if (row < 0 || row >= m_itens.size()) {
        return QVariant();
    }

    const auto &item = m_itens.at(row);

    if (role == Qt::DisplayRole) {
        switch (column) {
            case 0: return QString::number(item.numeroItem());
            case 1: return item.codigoProduto();
            case 2: return item.descricao();
            case 3: return item.codigoBarras().isEmpty() ? QString("-") : item.codigoBarras();
            case 4: return item.unidadeMedida();
            case 5: return item.vinculado() ? QString("Vinculado") : QString("Pendente");
            case 6: {
                if (item.vinculado()) {
                    QString desc = item.produtoDescricao();
                    if (!item.produtoCodigoInterno().isEmpty()) {
                        desc = QString("[%1] %2").arg(item.produtoCodigoInterno(), desc);
                    }
                    return desc;
                }
                return QString("Nenhum produto vinculado");
            }
            case 7: {
                if (item.vinculado()) {
                    return QString("x%1 %2").arg(QString::number(item.multiplicador(), 'f', 2), item.siglaUnidadeMedidaInterna());
                }
                return QString("-");
            }
            default: return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole) {
        switch (column) {
            case 0:
            case 1:
            case 3:
            case 4:
            case 5:
            case 7:
                return static_cast<int>(Qt::AlignCenter);
            default:
                return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }

    if (role == Qt::ForegroundRole) {
        if (column == 5) {
            if (item.vinculado()) {
                return QBrush(QColor("#2E7D32")); // Verde
            } else {
                return QBrush(QColor("#E65100")); // Laranja
            }
        }
        if (column == 6 && !item.vinculado()) {
            return QBrush(QColor("#757575")); // Cinza
        }
    }

    if (role == Qt::FontRole && column == 5) {
        QFont font;
        font.setBold(true);
        return font;
    }

    return QVariant();
}

QString NotaTerceiroItemListProvider::headerData(int column) const
{
    if (column < 0 || column >= m_headers.size()) {
        return QString();
    }
    return m_headers.at(column);
}

std::optional<NotaTerceiroItem> NotaTerceiroItemListProvider::getItemAt(int row) const
{
    if (row < 0 || row >= m_itens.size()) {
        return std::nullopt;
    }
    return m_itens.at(row);
}

std::optional<int> NotaTerceiroItemListProvider::columnWidth(int column) const
{
    switch (column) {
        case 0: return 70;   // Item #
        case 1: return 120;  // Cód. Fornecedor
        case 2: return 260;  // Descrição
        case 3: return 130;  // Cód. Barras
        case 4: return 80;   // Unid. Forn.
        case 5: return 100;  // Status
        case 6: return 260;  // Produto Interno Vinculado
        case 7: return 120;  // Fator / Unidade
        default: return std::nullopt;
    }
}

std::optional<QHeaderView::ResizeMode> NotaTerceiroItemListProvider::columnResizeMode(int column) const
{
    switch (column) {
        case 0: return QHeaderView::ResizeToContents;
        case 1: return QHeaderView::ResizeToContents;
        case 2: return QHeaderView::Stretch;
        case 3: return QHeaderView::ResizeToContents;
        case 4: return QHeaderView::ResizeToContents;
        case 5: return QHeaderView::ResizeToContents;
        case 6: return QHeaderView::Interactive;
        case 7: return QHeaderView::ResizeToContents;
        default: return std::nullopt;
    }
}

