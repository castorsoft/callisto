#include "FormPessoas.h"
#include "FormPessoaListagem.h"
#include "FormPessoaDetalhe.h"

FormPessoas::FormPessoas(QWidget *parent) : FormMultiViewBase(parent) {
    setListagem(new FormPessoaListagem(this));
}

FormDetalheBase* FormPessoas::criarFormDetalhe() {
    return new FormPessoaDetalhe(this);
}

void FormPessoas::onEditarRegistro(int rowIndex) {
    int id = m_formListagem->getIdSelecionado(rowIndex);
    if (id > 0) {
        abrirDetalhe(id);
    }
}

void FormPessoas::onDeletarRegistro(int rowIndex) {

}
