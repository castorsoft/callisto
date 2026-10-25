#include "FormProdutos.h"
#include "FormProdutoListagem.h"
#include "FormProdutoDetalhe.h"

FormProdutos::FormProdutos(QWidget *parent) : FormMultiViewBase(parent) {
    setListagem(new FormProdutoListagem(this));
}

FormDetalheBase* FormProdutos::criarFormDetalhe() {
    return new FormProdutoDetalhe(this);
}

void FormProdutos::onEditarRegistro(int rowIndex) {
    int id = m_formListagem->getIdSelecionado(rowIndex);
    if (id > 0) {
        abrirDetalhe(id);
    }
}
