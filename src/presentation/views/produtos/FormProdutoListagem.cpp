#include "FormProdutoListagem.h"
#include "../../../infrastructure/database/repositories/ProdutoRepository.h"
#include "../../components/providers/TableDataProviderAdapter.h"

FormProdutoListagem::FormProdutoListagem(QWidget *parent)
    : FormListagemBase(parent)
{
    carregarLista();
}

void FormProdutoListagem::carregarLista()
{
    ProdutoRepository repo;
    auto produtos = repo.listarTodos();
    m_provider = std::make_shared<ProdutoListProvider>(produtos);
    auto adapter = new TableDataProviderAdapter(m_provider, this);
    setModel(adapter);
}

int FormProdutoListagem::getIdSelecionado(int rowIndex)
{
    if (m_provider) {
        auto prod = m_provider->getProdutoAt(rowIndex);
        if (prod.has_value() && prod.value().id().has_value()) {
            return prod.value().id().value();
        }
    }
    return -1;
}
