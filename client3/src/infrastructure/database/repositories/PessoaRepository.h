#pragma once

#include <QSqlDatabase>
#include "../../../core/interfaces/IPessoaRepository.h"
#include "../../../core/entities/Pessoa.h"

class PessoaRepository : public IPessoaRepository {
public:
    PessoaRepository();

    void salvar(Pessoa &modelPessoa) override;
    std::optional<Pessoa> buscarPorId(int id) override;
    std::optional<Pessoa> buscarPorUuid(const QUuid &uuid) override;
    std::optional<Pessoa> buscarPorCnpjCpf(const QString &cnpjCpf) override;
    QList<Pessoa> listarTodas() override;
    bool deletar(int id) override;

private:
    std::optional<Pessoa> mapToEntity(class QSqlQuery &query);
};
