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
    auto listagem = qobject_cast<FormPessoaListagem*>(m_formListagem);
    if (listagem) {
        int id = listagem->getIdSelecionado(rowIndex);
        if (id > 0) {
            abrirDetalhe(id);
        }
    }
}
