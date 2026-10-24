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
    int id = m_formListagem->getIdSelecionado(rowIndex);
    if (id > 0) {
        abrirDetalhe(id);
    }
}
