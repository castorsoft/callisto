#pragma once
#include <QString>
#include <QVariant>
#include <QHeaderView>
#include <optional>

class ITableDataProvider {
public:
    virtual ~ITableDataProvider() = default;

    virtual int rowCount() const = 0;
    virtual int columnCount() const = 0;
    virtual QVariant data(int row, int column) const = 0;
    virtual QString headerData(int column) const = 0;

    virtual std::optional<int> columnWidth(int /*column*/) const {
        return std::nullopt;
    }

    virtual std::optional<QHeaderView::ResizeMode> columnResizeMode(int /*column*/) const {
        return std::nullopt;
    }
};
