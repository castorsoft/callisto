#pragma once

#include <QList>
#include <optional>
#include "../entities/Cidade.h"

class ICidadeRepository {
public:
    virtual ~ICidadeRepository() = default;
    virtual QList<Cidade> listarPorEstado(int idEstado) = 0;
    virtual std::optional<Cidade> buscarPorId(int id) = 0;
    virtual std::optional<Cidade> buscarPorIbge(int idIbge) = 0;
};
