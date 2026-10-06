#pragma once
#include <QList>
#include <QStringList>
#include "../../../core/entities/Pessoa.h"
#include "../../components/providers/ITableDataProvider.h"

class PessoaListProvider : public ITableDataProvider {
public:
    explicit PessoaListProvider(const QList<Pessoa>& Pessoas);

    int rowCount() const override;
    int columnCount() const override;
    QVariant data(int row, int column) const override;
    QString headerData(int column) const override;
    std::optional<int> columnWidth(int column) const override;

    std::optional<Pessoa> getPessoaAt(int row) const;

private:
    QList<Pessoa> m_pessoas;
    QStringList m_headers;
};
