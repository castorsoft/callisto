#include "ArquivoRepository.h"

#include <QSqlQuery>

#include "../DatabaseManager.h"
#include "../../../core/entities/Arquivo.h"

ArquivoRepository::ArquivoRepository()
{
    m_db = DatabaseManager::instance().database();
}

void ArquivoRepository::salvar(Arquivo &modelArquivo)
{
    QSqlQuery query(m_db);
    if (modelArquivo.id() == -1) {
        query.prepare("INSERT INTO arquivos (caminho_arquivo, nome_arquivo, processado, possui_erro, mensagem_erro, processar_novamente)"
            "VALUES (:caminho_arquivo, :nome_arquivo, :processado, :possui_erro, :mensagem_erro, :processar_novamente)"
            "RETURNING id, uuid, criado_em, atualizado_em");

        query.bindValue(":caminho_arquivo", modelArquivo.caminhoArquivo());
        query.bindValue(":nome_arquivo", modelArquivo.nomeArquivo());
        query.bindValue(":processado", modelArquivo.processado());
        query.bindValue(":possui_erro", modelArquivo.possuiErro());
        query.bindValue(":mensagem_erro", modelArquivo.mensagemErro().has_value() ? modelArquivo.mensagemErro().value() : QVariant());
        query.bindValue(":processar_novamente", modelArquivo.processarNovamente().has_value() ? modelArquivo.processarNovamente().value() : QVariant());

        if (query.exec() && query.next()) {
            modelArquivo.hidratar(query.value("id").toInt(), query.value("uuid").toUuid(), query.value("criado_em").toDateTime(), query.value("atualizado_em").toDateTime());
        }
    } else {
        // TODO: fazer update (não esquecer do returning do atualizado_em)
    }

}

bool ArquivoRepository::existeArquivo(const QString &filePath, Arquivo *modelArquivo)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM arquivos WHERE caminho_arquivo = :caminho_arquivo");

    query.bindValue(":caminho_arquivo", filePath);

    if (query.exec() && query.next()) {
        *modelArquivo = Arquivo::loadFromDatabase(
            query.value("id").toInt(),
            query.value("uuid").toUuid(),
            query.value("caminho_arquivo").toString(),
            query.value("nome_arquivo").toString(),
            query.value("processado").toBool(),
            query.value("possui_erro").toBool(),
            query.value("mensagem_erro").isNull() ? std::nullopt : std::make_optional(query.value("mensagem_erro").toString()),
            query.value("processar_novamente").isNull() ? std::nullopt : std::make_optional(query.value("processar_novamente").toBool()),
            query.value("criado_em").toDateTime(),
            query.value("atualizado_em").toDateTime()
        );
        return true;
    }

    return false;
}

QList<Arquivo> ArquivoRepository::carregarArquivosPendentes()
{
    QSqlQuery query(m_db);

    query.prepare("SELECT * FROM arquivos WHERE processado = false and (possui_erro = false OR processar_novamente = true)");

    if (query.exec() && query.next()) {

    }

    return QList<Arquivo>();
}
