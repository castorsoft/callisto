#pragma once

#include <optional>
#include <QUuid>
#include <QList>
#include "../entities/NCM.h"

class INCMRepository {
public:
    virtual ~INCMRepository() = default;

    virtual bool salvar(NCM &NCM) = 0;
    // virtual std::optional<NCM> buscarPorId(int id) = 0;
    // virtual std::optional<NCM> buscarPorUuid(const QUuid &uuid) = 0;
    virtual std::optional<NCM> buscarPorCodigo(const QString &codigo) = 0;
    // virtual QList<NCM> listarTodos() = 0;
};
