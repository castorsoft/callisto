#include "EnderecoRepository.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include "../DatabaseManager.h"
#include "CidadeRepository.h"

EnderecoRepository::EnderecoRepository()
{
    m_db = DatabaseManager::instance().database();
}

std::optional<Endereco> EnderecoRepository::mapToEntity(QSqlQuery &query)
{
    auto endereco = Endereco::loadFromDatabase(
        query.value("id").toInt(),
        query.value("uuid").toUuid(),
        query.value("logradouro").toString(),
        query.value("numero").isNull() ? std::nullopt : std::make_optional(query.value("numero").toString()),
        query.value("complemento").isNull() ? std::nullopt : std::make_optional(query.value("complemento").toString()),
        query.value("bairro").toString(),
        query.value("cep").toString(),
        query.value("id_cidade").toInt(),
        query.value("criado_em").toDateTime(),
        query.value("atualizado_em").toDateTime()
    );

    CidadeRepository cidRepo;
    auto cidade = cidRepo.buscarPorId(endereco.idCidade());
    if (cidade.has_value()) {
        endereco.setCidade(cidade.value());
    }

    return endereco;
}

void EnderecoRepository::salvar(Endereco &modelEndereco)
{
    QSqlQuery query(m_db);

    if (!modelEndereco.id().has_value() || modelEndereco.id().value() == -1) {
        query.prepare("INSERT INTO enderecos (logradouro, numero, complemento, bairro, cep, id_cidade) "
                      "VALUES (:logradouro, :numero, :complemento, :bairro, :cep, :id_cidade) "
                      "RETURNING id, uuid, criado_em, atualizado_em");
    } else {
        query.prepare("UPDATE enderecos SET logradouro = :logradouro, numero = :numero, complemento = :complemento, "
                      "bairro = :bairro, cep = :cep, id_cidade = :id_cidade, atualizado_em = CURRENT_TIMESTAMP "
                      "WHERE id = :id "
                      "RETURNING atualizado_em");
        query.bindValue(":id", modelEndereco.id().value());
    }

    query.bindValue(":logradouro", modelEndereco.logradouro());
    query.bindValue(":numero", modelEndereco.numero().has_value() ? modelEndereco.numero().value() : QVariant());
    query.bindValue(":complemento", modelEndereco.complemento().has_value() ? modelEndereco.complemento().value() : QVariant());
    query.bindValue(":bairro", modelEndereco.bairro());
    query.bindValue(":cep", modelEndereco.cep());
    query.bindValue(":id_cidade", modelEndereco.idCidade());

    if (query.exec() && query.next()) {
        if (!modelEndereco.id().has_value() || modelEndereco.id().value() == -1) {
            modelEndereco.hidratar(query.value("id").toInt(), query.value("uuid").toUuid(), query.value("criado_em").toDateTime(), query.value("atualizado_em").toDateTime());
        } else {
            modelEndereco.hidratar(query.value("atualizado_em").toDateTime());
        }
    }
}

std::optional<Endereco> EnderecoRepository::buscarPorId(int id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM enderecos WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return mapToEntity(query);
    }
    return std::nullopt;
}

std::optional<Endereco> EnderecoRepository::buscarPorUuid(const QUuid &uuid)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM enderecos WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);

    if (query.exec() && query.next()) {
        return mapToEntity(query);
    }
    return std::nullopt;
}

bool EnderecoRepository::deletar(int id)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM enderecos WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}
