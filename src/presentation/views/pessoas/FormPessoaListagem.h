#pragma once

#include "../../components/FormListagemBase.h"
#include "../providers/PessoaListProvider.h"
#include <memory>

class FormPessoaListagem : public FormListagemBase
{
    Q_OBJECT

public:
    explicit FormPessoaListagem(QWidget *parent = nullptr);
    ~FormPessoaListagem() override = default;

    void carregarLista() override;
    int getIdSelecionado(int rowIndex) override;

private:
    std::shared_ptr<PessoaListProvider> m_provider;
};
