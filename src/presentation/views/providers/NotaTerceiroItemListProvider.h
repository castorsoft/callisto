#pragma once

#include <QList>
#include <QStringList>
#include <optional>
#include "../../../core/entities/virtual/NotaTerceiroItem.h"
#include "../../components/providers/ITableDataProvider.h"

class NotaTerceiroItemListProvider : public ITableDataProvider {
public:
    explicit NotaTerceiroItemListProvider(const QList<NotaTerceiroItem> &itens);

    int rowCount() const override;
    int columnCount() const override;
    QVariant data(int row, int column) const override;
    QVariant data(int row, int column, int role) const override;
    QString headerData(int column) const override;
    std::optional<int> columnWidth(int column) const override;
    std::optional<QHeaderView::ResizeMode> columnResizeMode(int column) const override;

    std::optional<NotaTerceiroItem> getItemAt(int row) const;

private:
    QList<NotaTerceiroItem> m_itens;
    QStringList m_headers;
};

