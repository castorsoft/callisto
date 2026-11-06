#include "FormPessoaListagem.h"
#include "../../../core/RepositoryProvider.h"
#include "../../components/providers/TableDataProviderAdapter.h"

FormPessoaListagem::FormPessoaListagem(QWidget *parent)
    : FormListagemBase(parent)
{
    carregarLista();
}

void FormPessoaListagem::carregarLista()
{
    auto &repo = RepositoryProvider::pessoaRepository();
    auto Pessoas = repo.listarTodas();
    m_provider = std::make_shared<PessoaListProvider>(Pessoas);
    auto adapter = new TableDataProviderAdapter(m_provider, this);
    setModel(adapter);
}

int FormPessoaListagem::getIdSelecionado(int rowIndex)
{
    if (m_provider) {
        auto emp = m_provider->getPessoaAt(rowIndex);
        if (emp.has_value() && emp.value().id().has_value()) {
            return emp.value().id().value();
        }
    }
    return -1;
}
