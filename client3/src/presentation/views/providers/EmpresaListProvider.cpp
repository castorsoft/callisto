#include "EmpresaListProvider.h"

EmpresaListProvider::EmpresaListProvider(const QList<Empresa>& empresas)
    : m_empresas(empresas)
{
    m_headers << "ID" << "CNPJ/CPF" << "Razão Social" << "Nome Fantasia" << "Criado Em";
}

int EmpresaListProvider::rowCount() const
{
    return m_empresas.size();
}

int EmpresaListProvider::columnCount() const
{
    return m_headers.size();
}

QVariant EmpresaListProvider::data(int row, int column) const
{
    if (row < 0 || row >= m_empresas.size()) return QVariant();

    const Empresa& emp = m_empresas.at(row);

    switch (column) {
        case 0: return emp.id().value_or(-1);
        case 1: return emp.cnpjCpf();
        case 2: return emp.razaoSocial();
        case 3: return emp.nomeFantasia().value_or("");
        case 4: return emp.criadoEm().has_value() ? emp.criadoEm().value() : QVariant("");
        default: return QVariant();
    }
}

QString EmpresaListProvider::headerData(int column) const
{
    if (column < 0 || column >= m_headers.size()) return QString();
    return m_headers.at(column);
}

std::optional<Empresa> EmpresaListProvider::getEmpresaAt(int row) const
{
    if (row < 0 || row >= m_empresas.size()) return std::nullopt;
    return m_empresas.at(row);
}

std::optional<int> EmpresaListProvider::columnWidth(int column) const
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
