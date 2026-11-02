#include "NotaTerceiroListProvider.h"

#include <QColor>
#include <QBrush>
#include <QFont>

NotaTerceiroListProvider::NotaTerceiroListProvider(const QList<NotaTerceiroResumo> &notas)
    : m_notas(notas)
{
    m_headers << "#"
              << "NF-e / Série"
              << "Emissão"
              << "Fornecedor"
              << "CNPJ / CPF"
              << "Chave de Acesso"
              << "Pendentes / Total"
              << "Status";
}

int NotaTerceiroListProvider::rowCount() const
{
    return m_notas.size();
}

int NotaTerceiroListProvider::columnCount() const
{
    return m_headers.size();
}

QVariant NotaTerceiroListProvider::data(int row, int column) const
{
    return data(row, column, Qt::DisplayRole);
}

QVariant NotaTerceiroListProvider::data(int row, int column, int role) const
{
    if (row < 0 || row >= m_notas.size()) {
        return QVariant();
    }

    const auto &nota = m_notas.at(row);

    if (role == Qt::DisplayRole) {
        switch (column) {
            case 0: return QString::number(row + 1);
            case 1: return QString("%1 / %2").arg(nota.numero()).arg(nota.serie());
            case 2: return nota.dataEmissao().isValid() ? nota.dataEmissao().toString("dd/MM/yyyy HH:mm") : QVariant("");
            case 3: return nota.fornecedorNome();
            case 4: return nota.fornecedorCnpj();
            case 5: return nota.chaveAcesso();
            case 6: return QString("%1 / %2").arg(nota.itensPendentes()).arg(nota.totalItens());
            case 7: return (nota.itensPendentes() > 0) ? QString("Pendente") : QString("Concluído");
            default: return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole) {
        switch (column) {
            case 0:
            case 1:
            case 2:
            case 4:
            case 6:
            case 7:
                return static_cast<int>(Qt::AlignCenter);
            default:
                return static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter);
        }
    }

    if (role == Qt::ForegroundRole && column == 7) {
        if (nota.itensPendentes() > 0) {
            return QBrush(QColor("#E65100")); // Laranja
        } else {
            return QBrush(QColor("#2E7D32")); // Verde
        }
    }

    if (role == Qt::FontRole && column == 7) {
        QFont font;
        font.setBold(true);
        return font;
    }

    if (role == Qt::ToolTipRole && column == 5) {
        return nota.chaveAcesso();
    }

    return QVariant();
}

QString NotaTerceiroListProvider::headerData(int column) const
{
    if (column < 0 || column >= m_headers.size()) {
        return QString();
    }
    return m_headers.at(column);
}

std::optional<NotaTerceiroResumo> NotaTerceiroListProvider::getNotaAt(int row) const
{
    if (row < 0 || row >= m_notas.size()) {
        return std::nullopt;
    }
    return m_notas.at(row);
}

int NotaTerceiroListProvider::findRowById(int idDocumentoFiscal) const
{
    for (int i = 0; i < m_notas.size(); ++i) {
        if (m_notas.at(i).idDocumentoFiscal() == idDocumentoFiscal) {
            return i;
        }
    }
    return -1;
}

std::optional<int> NotaTerceiroListProvider::columnWidth(int column) const
{
    switch (column) {
        case 0: return 50;   // #
        case 1: return 110;  // NF-e / Série
        case 2: return 140;  // Emissão
        case 3: return 240;  // Fornecedor
        case 4: return 150;  // CNPJ / CPF
        case 5: return 220;  // Chave de Acesso
        case 6: return 140;  // Pendentes / Total
        case 7: return 100;  // Status
        default: return std::nullopt;
    }
}

std::optional<QHeaderView::ResizeMode> NotaTerceiroListProvider::columnResizeMode(int column) const
{
    switch (column) {
        case 0: return QHeaderView::ResizeToContents;
        case 1: return QHeaderView::ResizeToContents;
        case 2: return QHeaderView::ResizeToContents;
        case 3: return QHeaderView::Stretch;
        case 4: return QHeaderView::ResizeToContents;
        case 5: return QHeaderView::Interactive;
        case 6: return QHeaderView::ResizeToContents;
        case 7: return QHeaderView::ResizeToContents;
        default: return std::nullopt;
    }
}

