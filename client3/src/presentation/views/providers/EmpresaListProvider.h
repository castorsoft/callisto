#pragma once
#include <QList>
#include <QStringList>
#include "../../../core/entities/Empresa.h"
#include "../../components/providers/ITableDataProvider.h"

class EmpresaListProvider : public ITableDataProvider {
public:
    explicit EmpresaListProvider(const QList<Empresa>& empresas);

    int rowCount() const override;
    int columnCount() const override;
    QVariant data(int row, int column) const override;
    QString headerData(int column) const override;

    std::optional<Empresa> getEmpresaAt(int row) const;

private:
    QList<Empresa> m_empresas;
    QStringList m_headers;
};
