#include "FormEmpresas.h"
#include "FormEmpresaListagem.h"
#include "FormEmpresaDetalhe.h"

FormEmpresas::FormEmpresas(QWidget *parent) : FormMultiViewBase(parent) {
    setListagem(new FormEmpresaListagem(this));
}

FormDetalheBase* FormEmpresas::criarFormDetalhe() {
    return new FormEmpresaDetalhe(this);
}

void FormEmpresas::onEditarRegistro(int rowIndex) {
    auto listagem = qobject_cast<FormEmpresaListagem*>(m_formListagem);
    if (listagem) {
        int id = listagem->getIdSelecionado(rowIndex);
        if (id > 0) {
            abrirDetalhe(id);
        }
    }
}
