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

bool ArquivoRepository::salvar(Arquivo &arquivo)
{
    QSqlQuery query(DatabaseManager::instance().database());

    if (!arquivo.id().has_value()) {
        query.prepare(  "INSERT INTO public.arquivos "
                        "(caminho_arquivo, nome_arquivo, processado, possui_erro, mensagem_erro, processar_novamente)"
                        "VALUES (:caminho_arquivo, :nome_arquivo, :processado, :possui_erro, :mensagem_erro, :processar_novamente)"
                        "RETURNING id, uuid, criado_em, atualizado_em");
    } else {
        query.prepare(  "UPDATE public.arquivos SET "
                        "caminho_arquivo = :caminho_arquivo, "
                        "nome_arquivo = :nome_arquivo, "
                        "processado = :processado, "
                        "possui_erro = :possui_erro, "
                        "mensagem_erro = :mensagem_erro, "
                        "processar_novamente = :processar_novamente "
                        "WHERE id = :id "
                        "RETURNING atualizado_em");

        query.bindValue(":id", arquivo.id().value());
    }

    query.bindValue(":caminho_arquivo", arquivo.caminhoArquivo());
    query.bindValue(":nome_arquivo", arquivo.nomeArquivo());
    query.bindValue(":processado", arquivo.processado());
    query.bindValue(":possui_erro", arquivo.possuiErro());
    query.bindValue(":mensagem_erro", arquivo.mensagemErro().has_value() ? arquivo.mensagemErro().value() : QVariant());
    query.bindValue(":processar_novamente", arquivo.processarNovamente().has_value() ? arquivo.processarNovamente().value() : QVariant());


    if (query.exec() && query.next()) {
        arquivo.setAtualizadoEm(query.value("atualizado_em").toDateTime());

        if (!arquivo.id().has_value()) {
            arquivo.setId(query.value("id").toInt());
            arquivo.setUuid(query.value("uuid").toUuid());
            arquivo.setCriadoEm(query.value("criado_em").toDateTime());
        }

        return true;
    } else {
        qCritical() << "[ArquivoRepository] Erro ao inserir/atualizar arquivo fiscal:" << query.lastError().text();

        return false;
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

QList<Arquivo> ArquivoRepository::carregarArquivosPendentes() {
    QList<Arquivo> lista;

    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare("SELECT * FROM arquivos WHERE processado = false and (possui_erro = false OR processar_novamente = true)");

    if (!query.exec()) {
        while (query.next()) {
            lista.append(Arquivo::fromRecord(query.record()));
        }
    } else {
        qCritical() << "[ArquivoRepository] Falha ao carregar arquivos pendentes:" << query.lastError().text();
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
