#pragma once

#include "../../components/FormListagemBase.h"
#include "../providers/ProdutoListProvider.h"
#include <memory>

class FormProdutoListagem : public FormListagemBase
{
    Q_OBJECT

public:
    explicit FormProdutoListagem(QWidget *parent = nullptr);
    ~FormProdutoListagem() override = default;

    void carregarLista() override;
    int getIdSelecionado(int rowIndex) override;

private:
    std::shared_ptr<ProdutoListProvider> m_provider;
};
