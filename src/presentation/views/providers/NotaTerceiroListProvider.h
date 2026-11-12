#pragma once

#include <QList>
#include <QStringList>
#include <optional>
#include "../../../core/entities/virtual/NotaTerceiroResumo.h"
#include "../../components/providers/ITableDataProvider.h"

class NotaTerceiroListProvider : public ITableDataProvider {
public:
    explicit NotaTerceiroListProvider(const QList<NotaTerceiroResumo> &notas);

    int rowCount() const override;
    int columnCount() const override;
    QVariant data(int row, int column) const override;
    QVariant data(int row, int column, int role) const override;
    QString headerData(int column) const override;
    std::optional<int> columnWidth(int column) const override;
    std::optional<QHeaderView::ResizeMode> columnResizeMode(int column) const override;

    std::optional<NotaTerceiroResumo> getNotaAt(int row) const;
    int findRowById(int idDocumentoFiscal) const;

private:
    QList<NotaTerceiroResumo> m_notas;
    QStringList m_headers;
};

