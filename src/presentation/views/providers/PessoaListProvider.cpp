#include "PessoaListProvider.h"

PessoaListProvider::PessoaListProvider(const QList<Pessoa>& pessoas)
    : m_pessoas(pessoas)
{
    m_headers << "ID" << "CNPJ/CPF" << "Razão Social" << "Nome Fantasia" << "Criado Em";
}

int PessoaListProvider::rowCount() const
{
    return m_pessoas.size();
}

int PessoaListProvider::columnCount() const
{
    return m_headers.size();
}

QVariant PessoaListProvider::data(int row, int column) const
{
    if (row < 0 || row >= m_pessoas.size()) return QVariant();

    const Pessoa& pes = m_pessoas.at(row);

    switch (column) {
        case 0: return pes.id().value_or(-1);
        case 1: return pes.cnpjCpf();
        case 2: return pes.razaoSocial();
        case 3: return pes.nomeFantasia().value_or("");
        case 4: return pes.criadoEm().has_value() ? pes.criadoEm().value() : QVariant("");
        default: return QVariant();
    }
}

QString PessoaListProvider::headerData(int column) const
{
    if (column < 0 || column >= m_headers.size()) return QString();
    return m_headers.at(column);
}

std::optional<Pessoa> PessoaListProvider::getPessoaAt(int row) const
{
    if (row < 0 || row >= m_pessoas.size()) return std::nullopt;
    return m_pessoas.at(row);
}

std::optional<int> PessoaListProvider::columnWidth(int column) const
{
    switch (column) {
        case 0: return 60;   // ID
        case 1: return 160;  // CNPJ/CPF
        case 2: return 260;  // Razão Social
        case 3: return 200;  // Nome Fantasia
        case 4: return 150;  // Criado Em
        default: return std::nullopt;
    }
}

std::optional<QHeaderView::ResizeMode> PessoaListProvider::columnResizeMode(int column) const
{
    switch (column) {
        case 0: return QHeaderView::ResizeToContents; // ID
        case 1: return QHeaderView::ResizeToContents; // CNPJ/CPF
        case 2: return QHeaderView::Stretch;          // Razão Social
        case 3: return QHeaderView::Interactive;      // Nome Fantasia
        case 4: return QHeaderView::ResizeToContents; // Criado Em
        default: return std::nullopt;
    }
}
