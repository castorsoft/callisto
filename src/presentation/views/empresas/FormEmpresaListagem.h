#ifndef FORMEMPRESALISTAGEM_H
#define FORMEMPRESALISTAGEM_H

#include "../../components/FormListagemBase.h"
#include "../providers/EmpresaListProvider.h"
#include <memory>

class FormEmpresaListagem : public FormListagemBase
{
    Q_OBJECT

public:
    explicit FormEmpresaListagem(QWidget *parent = nullptr);
    ~FormEmpresaListagem() override = default;

    void carregarLista() override;
    int getIdSelecionado(int rowIndex) override;

private:
    std::shared_ptr<EmpresaListProvider> m_provider;
};

#endif // FORMEMPRESALISTAGEM_H
