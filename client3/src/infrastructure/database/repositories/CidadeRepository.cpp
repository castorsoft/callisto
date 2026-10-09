#include "CidadeRepository.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include "../DatabaseManager.h"

CidadeRepository::CidadeRepository() {
}

QList<Cidade> CidadeRepository::listarPorEstado(int idEstado) {
    QList<Cidade> cidades;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, nome, id_ibge, id_estado FROM cidades WHERE id_estado = :id_estado ORDER BY nome");
    query.bindValue(":id_estado", idEstado);

    if (query.exec()) {
        while (query.next()) {
            cidades.append(Cidade(
                query.value("id").toInt(),
                query.value("nome").toString(),
                query.value("id_ibge").toInt(),
                query.value("id_estado").toInt()
            ));
        }
    }
    return cidades;
}

std::optional<Cidade> CidadeRepository::buscarPorId(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, nome, id_ibge, id_estado FROM cidades WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return Cidade(
            query.value("id").toInt(),
            query.value("nome").toString(),
            query.value("id_ibge").toInt(),
            query.value("id_estado").toInt()
        );
    }
    return std::nullopt;
}

std::optional<Cidade> CidadeRepository::buscarPorIbge(int idIbge) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, nome, id_ibge, id_estado FROM cidades WHERE id_ibge = :id_ibge");
    query.bindValue(":id_ibge", idIbge);

    if (query.exec() && query.next()) {
        return Cidade(
            query.value("id").toInt(),
            query.value("nome").toString(),
            query.value("id_ibge").toInt(),
            query.value("id_estado").toInt()
        );
    }
    return std::nullopt;
}
