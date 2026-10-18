#pragma once

#include <optional>
#include <QUuid>
#include <QList>
#include "../../../core/interfaces/IUnidadeMedidaRepository.h"
#include "../../../core/entities/UnidadeMedida.h"

class UnidadeMedidaRepository : public IUnidadeMedidaRepository {
public:
    UnidadeMedidaRepository() = default;
    ~UnidadeMedidaRepository() override = default;

    bool salvar(UnidadeMedida &unidadeMedida) override;
    std::optional<UnidadeMedida> buscarPorId(int id) override;
    std::optional<UnidadeMedida> buscarPorSigla(const QString &sigla) override;
    QList<UnidadeMedida> buscarPorTermo(const QString &termo) override;
    QList<UnidadeMedida> listarTodas() override;
};
