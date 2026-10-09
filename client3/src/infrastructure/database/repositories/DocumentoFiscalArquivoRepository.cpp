#include "DocumentoFiscalArquivoRepository.h"
#include "../DatabaseManager.h"
#include <QDebug>

DocumentoFiscalArquivoRepository::DocumentoFiscalArquivoRepository()
{
}

bool DocumentoFiscalArquivoRepository::salvar(DocumentoFiscalArquivo &arquivo)
{
    QSqlQuery query(DatabaseManager::instance().database());

    if (arquivo.id().has_value()) {
        query.prepare("UPDATE documentos_fiscais_arquivos SET "
                      "id_documento = :id_documento, "
                      "conteudo_xml = :conteudo_xml, "
                      "tipo_arquivo = :tipo_arquivo "
                      "WHERE id = :id "
                      "RETURNING atualizado_em");
        query.bindValue(":id_documento", arquivo.idDocumento());
        query.bindValue(":conteudo_xml", arquivo.conteudoXml());
        query.bindValue(":tipo_arquivo", arquivo.tipoArquivo());
        query.bindValue(":id", arquivo.id().value());

        if (query.exec() && query.next()) {
            arquivo.setAtualizadoEm(query.value("atualizado_em").toDateTime());
            return true;
        } else {
            qCritical() << "[DocumentoFiscalArquivoRepository] Erro ao atualizar arquivo fiscal:" << query.lastError().text();
            return false;
        }
    } else {
        query.prepare("INSERT INTO documentos_fiscais_arquivos "
                      "(id_documento, conteudo_xml, tipo_arquivo) "
                      "VALUES (:id_documento, :conteudo_xml, :tipo_arquivo) "
                      "RETURNING id, uuid, criado_em, atualizado_em");
        query.bindValue(":id_documento", arquivo.idDocumento());
        query.bindValue(":conteudo_xml", arquivo.conteudoXml());
        query.bindValue(":tipo_arquivo", arquivo.tipoArquivo());

        if (query.exec() && query.next()) {
            arquivo.hidratar(query.value("id").toInt(),
                             query.value("uuid").toUuid(),
                             query.value("criado_em").toDateTime(),
                             query.value("atualizado_em").toDateTime());
            return true;
        } else {
            qCritical() << "[DocumentoFiscalArquivoRepository] Erro ao inserir arquivo fiscal:" << query.lastError().text();
            return false;
        }
    }
}

std::optional<DocumentoFiscalArquivo> DocumentoFiscalArquivoRepository::buscarPorId(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, uuid, id_documento, conteudo_xml, tipo_arquivo, criado_em, atualizado_em "
                  "FROM documentos_fiscais_arquivos WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return mapFromQuery(query);
    }
    return std::nullopt;
}

std::optional<DocumentoFiscalArquivo> DocumentoFiscalArquivoRepository::buscarPrincipalPorIdDocumento(int idDocumento)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, uuid, id_documento, conteudo_xml, tipo_arquivo, criado_em, atualizado_em "
                  "FROM documentos_fiscais_arquivos "
                  "WHERE id_documento = :id_documento AND tipo_arquivo = 'DOCUMENTO_FISCAL' "
                  "ORDER BY id DESC LIMIT 1");
    query.bindValue(":id_documento", idDocumento);

    if (query.exec() && query.next()) {
        return mapFromQuery(query);
    }
    return std::nullopt;
}

QList<DocumentoFiscalArquivo> DocumentoFiscalArquivoRepository::listarPorIdDocumento(int idDocumento)
{
    QList<DocumentoFiscalArquivo> lista;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, uuid, id_documento, conteudo_xml, tipo_arquivo, criado_em, atualizado_em "
                  "FROM documentos_fiscais_arquivos "
                  "WHERE id_documento = :id_documento "
                  "ORDER BY id ASC");
    query.bindValue(":id_documento", idDocumento);

    if (query.exec()) {
        while (query.next()) {
            lista.append(mapFromQuery(query));
        }
    } else {
        qCritical() << "[DocumentoFiscalArquivoRepository] Erro ao listar arquivos do documento:" << query.lastError().text();
    }
    return lista;
}

DocumentoFiscalArquivo DocumentoFiscalArquivoRepository::mapFromQuery(const QSqlQuery &query) const
{
    return DocumentoFiscalArquivo::loadFromDatabase(
        query.value("id").toInt(),
        query.value("uuid").toUuid(),
        query.value("id_documento").toInt(),
        query.value("conteudo_xml").toString(),
        query.value("tipo_arquivo").toString(),
        query.value("criado_em").toDateTime(),
        query.value("atualizado_em").toDateTime()
    );
}
