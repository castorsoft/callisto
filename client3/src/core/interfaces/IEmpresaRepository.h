#pragma once

#include "../entities/Empresa.h"
#include <QList>
#include <QUuid>
#include <optional>

class IEmpresaRepository {
public:
    virtual ~IEmpresaRepository() = default;

    virtual void salvar(Empresa &modelEmpresa) = 0;
    virtual std::optional<Empresa> buscarPorId(int id) = 0;
    virtual std::optional<Empresa> buscarPorUuid(const QUuid &uuid) = 0;
    virtual std::optional<Empresa> buscarPorCnpjCpf(const QString &cnpjCpf) = 0;
    virtual QList<Empresa> listarTodas() = 0;
    virtual bool deletar(int id) = 0;
};
