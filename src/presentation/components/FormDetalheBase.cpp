#include "FormDetalheBase.h"

void FormDetalheBase::setIdRegistro(int id) {
    m_idRegistro = id;
    carregarDados();
}
