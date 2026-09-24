#pragma once

#include <QSqlDatabase>
#include "../../../core/interfaces/IEnderecoRepository.h"
#include "../../../core/entities/Endereco.h"

class EnderecoRepository : public IEnderecoRepository {
public:
    EnderecoRepository();

    void salvar(Endereco &modelEndereco) override;
    std::optional<Endereco> buscarPorId(int id) override;
    std::optional<Endereco> buscarPorUuid(const QUuid &uuid) override;
    bool deletar(int id) override;

private:
    QSqlDatabase m_db;

    std::optional<Endereco> mapToEntity(class QSqlQuery &query);
};
