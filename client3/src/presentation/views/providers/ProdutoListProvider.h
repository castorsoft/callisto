#pragma once
#include <QList>
#include <QStringList>
#include "../../../core/entities/Produto.h"
#include "../../components/providers/ITableDataProvider.h"

class ProdutoListProvider : public ITableDataProvider {
public:
    explicit ProdutoListProvider(const QList<Produto>& produtos);

    int rowCount() const override;
    int columnCount() const override;
    QVariant data(int row, int column) const override;
    QString headerData(int column) const override;
    std::optional<int> columnWidth(int column) const override;
    std::optional<QHeaderView::ResizeMode> columnResizeMode(int column) const override;

    std::optional<Produto> getProdutoAt(int row) const;

private:
    QList<Produto> m_produtos;
    QStringList m_headers;
};
