#include "ProdutoListProvider.h"

ProdutoListProvider::ProdutoListProvider(const QList<Produto>& produtos)
    : m_produtos(produtos)
{
    m_headers << "ID" << "Descrição" << "Cód. barras" << "Criado Em";
}

int ProdutoListProvider::rowCount() const
{
    return m_produtos.size();
}

int ProdutoListProvider::columnCount() const
{
    return m_headers.size();
}

QVariant ProdutoListProvider::data(int row, int column) const
{
    if (row < 0 || row >= m_produtos.size()) return QVariant();

    const Produto& prod = m_produtos.at(row);

    switch (column) {
        case 0: return prod.id().value_or(-1);
        case 1: return prod.descricao();
        case 2: return prod.codigoBarra().has_value() ? prod.codigoBarra().value() : QVariant("");
        case 3: return prod.criadoEm().has_value() ? prod.criadoEm().value() : QVariant("");
        default: return QVariant();
    }
}

QString ProdutoListProvider::headerData(int column) const
{
    if (column < 0 || column >= m_headers.size()) return QString();
    return m_headers.at(column);
}

std::optional<Produto> ProdutoListProvider::getProdutoAt(int row) const
{
    if (row < 0 || row >= m_produtos.size()) return std::nullopt;
    return m_produtos.at(row);
}

std::optional<int> ProdutoListProvider::columnWidth(int column) const
{
    switch (column) {
        case 0: return 60;   // ID
        case 1: return 300;  // Descricão
        case 2: return 160;  // Cód Barras
        case 3: return 150;  // Criado Em
        default: return std::nullopt;
    }
}
