#pragma once
#include <QString>
#include <QVariant>

class ITableDataProvider {
public:
    virtual ~ITableDataProvider() = default;

    virtual int rowCount() const = 0;
    virtual int columnCount() const = 0;
    virtual QVariant data(int row, int column) const = 0;
    virtual QString headerData(int column) const = 0;
};
