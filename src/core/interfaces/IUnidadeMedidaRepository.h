#pragma once

#include <optional>
#include <QUuid>
#include <QList>
#include "../entities/UnidadeMedida.h"

class IUnidadeMedidaRepository {
public:
    virtual ~IUnidadeMedidaRepository() = default;

    virtual bool salvar(UnidadeMedida &unidadeMedida) = 0;
    virtual std::optional<UnidadeMedida> buscarPorId(int id) = 0;
    // virtual std::optional<UnidadeMedida> buscarPorUuid(const QUuid &uuid) = 0;
    virtual std::optional<UnidadeMedida> buscarPorSigla(const QString &sigla) = 0;
    virtual QList<UnidadeMedida> buscarPorTermo(const QString &termo) = 0;
    virtual QList<UnidadeMedida> listarTodas() = 0;
};
