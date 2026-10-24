#pragma once

#include <QList>
#include "../entities/Estado.h"

class IEstadoRepository {
public:
    virtual ~IEstadoRepository() = default;
    virtual QList<Estado> listarTodos() = 0;
};
