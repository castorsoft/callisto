#pragma once

#include <QSqlDatabase>
#include "../../../core/interfaces/IEstadoRepository.h"
#include "../../../core/entities/Estado.h"

class EstadoRepository : public IEstadoRepository {
public:
    EstadoRepository();
    QList<Estado> listarTodos() override;
};
