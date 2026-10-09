#include "ArquivoRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

#include "../DatabaseManager.h"
#include "../../../core/entities/Arquivo.h"

ArquivoRepository::ArquivoRepository()
{
}

void ArquivoRepository::salvar(Arquivo &modelArquivo)
{
    QSqlQuery query(DatabaseManager::instance().database());
    if (!modelArquivo.id().has_value() || modelArquivo.id().value() == -1) {
        query.prepare("INSERT INTO arquivos (caminho_arquivo, nome_arquivo, processado, possui_erro, mensagem_erro, processar_novamente)"
            "VALUES (:caminho_arquivo, :nome_arquivo, :processado, :possui_erro, :mensagem_erro, :processar_novamente)"
            "RETURNING id, uuid, criado_em, atualizado_em");

        query.bindValue(":caminho_arquivo", modelArquivo.caminhoArquivo());
        query.bindValue(":nome_arquivo", modelArquivo.nomeArquivo());
        query.bindValue(":processado", modelArquivo.processado());
        query.bindValue(":possui_erro", modelArquivo.possuiErro());
        query.bindValue(":mensagem_erro", modelArquivo.mensagemErro().has_value() ? modelArquivo.mensagemErro().value() : QVariant());
        query.bindValue(":processar_novamente", modelArquivo.processarNovamente().has_value() ? modelArquivo.processarNovamente().value() : QVariant());

        if (query.exec()) {
            if (query.next()) {
                modelArquivo.setId(query.value("id").toInt());
                modelArquivo.setUuid(query.value("uuid").toUuid());
                modelArquivo.setCriadoEm(query.value("criado_em").toDateTime());
                modelArquivo.setAtualizadoEm(query.value("atualizado_em").toDateTime());
            } else {
                qCritical() << "[ArquivoRepository] INSERT em arquivos nao retornou registros RETURNING.";
            }
        } else {
            qCritical() << "[ArquivoRepository] Falha ao salvar arquivo no banco:" << query.lastError().text();
        }
    } else {
        modelArquivo.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        // TODO: fazer update (não esquecer do returning do atualizado_em)
    }

}

bool ArquivoRepository::existeArquivo(const QString &filePath, Arquivo *modelArquivo)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM arquivos WHERE caminho_arquivo = :caminho_arquivo");

    query.bindValue(":caminho_arquivo", filePath);

    if (!query.exec()) {
        qCritical() << "[ArquivoRepository] Falha ao verificar se arquivo existe:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        if (modelArquivo) {
            *modelArquivo = Arquivo::fromRecord(query.record());
        }
        return true;
    }

    return false;
}

QList<Arquivo> ArquivoRepository::carregarArquivosPendentes()
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare("SELECT * FROM arquivos WHERE processado = false and (possui_erro = false OR processar_novamente = true)");

    if (!query.exec()) {
        qCritical() << "[ArquivoRepository] Falha ao carregar arquivos pendentes:" << query.lastError().text();
        return QList<Arquivo>();
    }

    QList<Arquivo> lista;
    while (query.next()) {
        lista.append(Arquivo::fromRecord(query.record()));
    }

    return lista;
}

std::optional<Arquivo> ArquivoRepository::buscarPorUuid(const QUuid &uuid)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM arquivos WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);

    if (!query.exec()) {
        qCritical() << "[ArquivoRepository] Falha ao buscar arquivo por UUID:" << query.lastError().text();
        return std::nullopt;
    }

    if (query.next()) {
        return Arquivo::fromRecord(query.record());
    }
    return std::nullopt;
}
