#pragma once

#include <QSqlDatabase>
#include "../../../core/interfaces/ICidadeRepository.h"
#include "../../../core/entities/Cidade.h"

class CidadeRepository : public ICidadeRepository {
public:
    CidadeRepository();
    QList<Cidade> listarPorEstado(int idEstado) override;
    std::optional<Cidade> buscarPorId(int id);
    std::optional<Cidade> buscarPorIbge(int idIbge);
};
