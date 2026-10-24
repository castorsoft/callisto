#pragma once

#include <QList>
#include "../entities/Cidade.h"

class ICidadeRepository {
public:
    virtual ~ICidadeRepository() = default;
    virtual QList<Cidade> listarPorEstado(int idEstado) = 0;
};
