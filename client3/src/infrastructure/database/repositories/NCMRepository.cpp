#include "NCMRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

#include "../DatabaseManager.h"

bool NCMRepository::salvar(NCM &ncm)
{
    QSqlQuery query(DatabaseManager::instance().database());

    if (!ncm.id().has_value()) {
        query.prepare(  "INSERT INTO public.ncms "
                        "(codigo, descricao, aliquota_municipal, aliquota_estadual, aliquota_federal, data_vigencia_inicio, data_vigencia_fim) "
                        "VALUES (:codigo, :descricao, :aliquota_municipal, :aliquota_estadual, :aliquota_federal, :data_vigencia_inicio, :data_vigencia_fim) "
                        "RETURNING id, uuid, criado_em, atualizado_em");
    } else {
        query.prepare(  "UPDATE public.ncms SET "
                        "codigo = :codigo, "
                        "descricao = :descricao, "
                        "aliquota_municipal = :aliquota_municipal, "
                        "aliquota_estadual = :aliquota_estadual, "
                        "aliquota_federal = :aliquota_federal, "
                        "data_vigencia_inicio = :data_vigencia_inicio, "
                        "data_vigencia_fim = :data_vigencia_fim "
                        "WHERE id = :id "
                        "RETURNING atualizado_em");

        query.bindValue(":id", ncm.id().value());
    }

    query.bindValue(":codigo", ncm.codigo());
    query.bindValue(":descricao", ncm.descricao());
    query.bindValue(":aliquota_municipal", ncm.aliquotaMunicipal());
    query.bindValue(":aliquota_estadual", ncm.aliquotaEstadual());
    query.bindValue(":aliquota_federal", ncm.aliquotaFederal());
    query.bindValue(":data_vigencia_inicio", ncm.dataVigenciaInicio());
    query.bindValue(":data_vigencia_fim", ncm.dataVigenciaFim());

    if (query.exec() && query.next()) {
        ncm.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        if (!ncm.id().has_value()) {
            ncm.setId(query.value("id").toInt());
            ncm.setUuid(query.value("uuid").toUuid());
            ncm.setCriadoEm(query.value("criado_em").toDateTime());
        }

        return true;
    } else {
        qCritical() << "[NCMRepository] Erro ao inserir/atualizar ncm:" << query.lastError().text();

        return false;
    }
}

std::optional<NCM> NCMRepository::buscarPorCodigo(const QString &codigo)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.ncms WHERE codigo = :codigo");
    query.bindValue(":codigo", codigo);

    if (query.exec() && query.next()) {
        return NCM::fromRecord(query.record());
    }
    return std::nullopt;
}
