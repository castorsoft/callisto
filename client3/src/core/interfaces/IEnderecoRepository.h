#pragma once

#include "../entities/Endereco.h"
#include <QUuid>
#include <optional>

class IEnderecoRepository {
public:
    virtual ~IEnderecoRepository() = default;

    virtual void salvar(Endereco &modelEndereco) = 0;
    virtual std::optional<Endereco> buscarPorId(int id) = 0;
    virtual std::optional<Endereco> buscarPorUuid(const QUuid &uuid) = 0;
    virtual bool deletar(int id) = 0;
};
