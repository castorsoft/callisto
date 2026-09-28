#pragma once

#include <optional>
#include <QUuid>
#include <QList>
#include "../entities/Pessoa.h"

class IPessoaRepository {
public:
    virtual ~IPessoaRepository() = default;

    virtual void salvar(Pessoa &modelPessoa) = 0;
    virtual std::optional<Pessoa> buscarPorId(int id) = 0;
    virtual std::optional<Pessoa> buscarPorUuid(const QUuid &uuid) = 0;
    virtual std::optional<Pessoa> buscarPorCnpjCpf(const QString &cnpjCpf) = 0;
    virtual QList<Pessoa> listarTodas() = 0;
    virtual bool deletar(int id) = 0;
};
