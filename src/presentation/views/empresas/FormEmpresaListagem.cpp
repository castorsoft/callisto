#include "FormEmpresaListagem.h"
#include "../../../core/RepositoryProvider.h"
#include "../../components/providers/TableDataProviderAdapter.h"

FormEmpresaListagem::FormEmpresaListagem(QWidget *parent)
    : FormListagemBase(parent)
{
    carregarLista();
}

void FormEmpresaListagem::carregarLista()
{
    auto &repo = RepositoryProvider::empresaRepository();
    auto empresas = repo.listarTodas();
    m_provider = std::make_shared<EmpresaListProvider>(empresas);
    auto adapter = new TableDataProviderAdapter(m_provider, this);
    setModel(adapter);
}

int FormEmpresaListagem::getIdSelecionado(int rowIndex)
{
    if (m_provider) {
        auto emp = m_provider->getEmpresaAt(rowIndex);
        if (emp.has_value() && emp.value().id().has_value()) {
            return emp.value().id().value();
        }
    }
    return -1;
}
