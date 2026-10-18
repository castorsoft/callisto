#include "UnidadeMedidaRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

#include "../DatabaseManager.h"

bool UnidadeMedidaRepository::salvar(UnidadeMedida &unidadeMedida)
{
    QSqlQuery query(DatabaseManager::instance().database());

    if (!unidadeMedida.id().has_value()) {
        query.prepare(  "INSERT INTO public.unidades_medida "
                        "(sigla, descricao) "
                        "VALUES (:sigla, :descricao) "
                        "RETURNING id, uuid, criado_em, atualizado_em");
    } else {
        query.prepare(  "UPDATE public.unidades_medida SET "
                        "sigla = :sigla, "
                        "descricao = :descricao "
                        "WHERE id = :id "
                        "RETURNING atualizado_em");

        query.bindValue(":id", unidadeMedida.id().value());
    }

    query.bindValue(":sigla", unidadeMedida.sigla());
    query.bindValue(":descricao", unidadeMedida.descricao());

    if (query.exec() && query.next()) {
        unidadeMedida.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        if (!unidadeMedida.id().has_value()) {
            unidadeMedida.setId(query.value("id").toInt());
            unidadeMedida.setUuid(query.value("uuid").toUuid());
            unidadeMedida.setCriadoEm(query.value("criado_em").toDateTime());
        }

        return true;
    } else {
        qCritical() << "[UnidadeMedidaRepository] Erro ao inserir/atualizar unidade de medida:" << query.lastError().text();

        return false;
    }
}

std::optional<UnidadeMedida> UnidadeMedidaRepository::buscarPorId(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.unidades_medida WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return UnidadeMedida::fromRecord(query.record());
    }
    return std::nullopt;
}

std::optional<UnidadeMedida> UnidadeMedidaRepository::buscarPorSigla(const QString &sigla)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.unidades_medida WHERE sigla = :sigla");
    query.bindValue(":sigla", sigla);

    if (query.exec() && query.next()) {
        return UnidadeMedida::fromRecord(query.record());
    }
    return std::nullopt;
}

QList<UnidadeMedida> UnidadeMedidaRepository::buscarPorTermo(const QString &termo)
{
    QList<UnidadeMedida> unMedidas;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.unidades_medida WHERE sigla ILIKE :termo OR descricao ILIKE :termo");
    query.bindValue(":termo", "%" + termo + "%");

    if (query.exec()) {
        while (query.next()) {
            unMedidas.append(UnidadeMedida::fromRecord(query.record()));
        }
    }

    return unMedidas;
}

QList<UnidadeMedida> UnidadeMedidaRepository::listarTodas()
{
    QList<UnidadeMedida> lista;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.unidades_medida ORDER BY sigla ASC");

    if (query.exec()) {
        while (query.next()) {
            lista.append(UnidadeMedida::fromRecord(query.record()));
        }
    } else {
        qCritical() << "[UnidadeMedidaRepository] Erro ao listar unidades de medida:" << query.lastError().text();
    }
    return lista;
}
