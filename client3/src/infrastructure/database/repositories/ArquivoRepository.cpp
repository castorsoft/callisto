#include "ArquivoRepository.h"

#include <QSqlQuery>

#include "../DatabaseManager.h"
#include "../../../core/entities/Arquivo.h"

#include <QSqlRecord>

ArquivoRepository::ArquivoRepository()
{
    m_db = DatabaseManager::instance().database();
}

void ArquivoRepository::salvar(Arquivo &modelArquivo)
{
    QSqlQuery query(m_db);
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

        if (query.exec() && query.next()) {
            modelArquivo.setId(query.value("id").toInt());
            modelArquivo.setUuid(query.value("uuid").toUuid());
            modelArquivo.setCriadoEm(query.value("criado_em").toDateTime());
            modelArquivo.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        }
    } else {
        modelArquivo.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        // TODO: fazer update (não esquecer do returning do atualizado_em)
    }

}

bool ArquivoRepository::existeArquivo(const QString &filePath, Arquivo *modelArquivo)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM arquivos WHERE caminho_arquivo = :caminho_arquivo");

    query.bindValue(":caminho_arquivo", filePath);

    if (query.exec() && query.next()) {
        if (modelArquivo) {
            *modelArquivo = Arquivo::fromRecord(query.record());
        }
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

std::optional<Arquivo> ArquivoRepository::buscarPorUuid(const QUuid &uuid)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM arquivos WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);

    if (query.exec() && query.next()) {
        return Arquivo::fromRecord(query.record());
    }
    return std::nullopt;
}
