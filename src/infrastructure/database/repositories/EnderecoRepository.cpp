#include "EnderecoRepository.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QSqlRecord>
#include "../DatabaseManager.h"
#include "CidadeRepository.h"

EnderecoRepository::EnderecoRepository()
{
}

std::optional<Endereco> EnderecoRepository::mapToEntity(QSqlQuery &query)
{
    auto endereco = Endereco::fromRecord(query.record());

    CidadeRepository cidRepo;
    auto cidade = cidRepo.buscarPorId(endereco.idCidade());
    if (cidade.has_value()) {
        endereco.setCidade(cidade.value());
    }

    return endereco;
}

void EnderecoRepository::salvar(Endereco &endereco)
{
    QSqlQuery query(DatabaseManager::instance().database());

    if (!endereco.id().has_value()) {
        query.prepare("INSERT INTO public.enderecos (logradouro, numero, complemento, bairro, cep, id_cidade) "
                      "VALUES (:logradouro, :numero, :complemento, :bairro, :cep, :id_cidade) "
                      "RETURNING id, uuid, criado_em, atualizado_em");
    } else {
        query.prepare(  "UPDATE public.enderecos SET "
                        "logradouro = :logradouro, "
                        "numero = :numero, "
                        "complemento = :complemento, "
                        "bairro = :bairro, "
                        "cep = :cep, "
                        "id_cidade = :id_cidade "
                        "WHERE id = :id "
                        "RETURNING atualizado_em");

        query.bindValue(":id", endereco.id().value());
    }

    query.bindValue(":logradouro", endereco.logradouro());
    query.bindValue(":numero", endereco.numero().has_value() ? endereco.numero().value() : QVariant());
    query.bindValue(":complemento", endereco.complemento().has_value() ? endereco.complemento().value() : QVariant());
    query.bindValue(":bairro", endereco.bairro());
    query.bindValue(":cep", endereco.cep());
    query.bindValue(":id_cidade", endereco.idCidade());

    if (query.exec() && query.next()) {
        if (!endereco.id().has_value() || endereco.id().value() == -1) {
            endereco.setId(query.value("id").toInt());
            endereco.setUuid(query.value("uuid").toUuid());
            endereco.setCriadoEm(query.value("criado_em").toDateTime());
            endereco.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        } else {
            endereco.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        }
    }
}

std::optional<Endereco> EnderecoRepository::buscarPorId(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM enderecos WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return mapToEntity(query);
    }
    return std::nullopt;
}

std::optional<Endereco> EnderecoRepository::buscarPorUuid(const QUuid &uuid)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM enderecos WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);

    if (query.exec() && query.next()) {
        return mapToEntity(query);
    }
    return std::nullopt;
}

bool EnderecoRepository::deletar(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM enderecos WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}
