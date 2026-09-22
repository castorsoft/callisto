#pragma once
#include <QAbstractTableModel>
#include <memory>
#include "ITableDataProvider.h"

class TableDataProviderAdapter : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit TableDataProviderAdapter(std::shared_ptr<ITableDataProvider> provider, QObject* parent = nullptr)
        : QAbstractTableModel(parent), m_provider(provider) {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override {
        if (parent.isValid() || !m_provider) return 0;
        return m_provider->rowCount();
    }

    int columnCount(const QModelIndex& parent = QModelIndex()) const override {
        if (parent.isValid() || !m_provider) return 0;
        return m_provider->columnCount();
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (!index.isValid() || !m_provider) return QVariant();
        if (role == Qt::DisplayRole) {
            return m_provider->data(index.row(), index.column());
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal && m_provider) {
            return m_provider->headerData(section);
        }
        return QAbstractTableModel::headerData(section, orientation, role);
    }

private:
    std::shared_ptr<ITableDataProvider> m_provider;
};
