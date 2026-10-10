#include "ProdutoRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>

#include "../DatabaseManager.h"

bool ProdutoRepository::salvar(Produto &produto)
{
    QSqlQuery query(DatabaseManager::instance().database());

    if (!produto.id().has_value()) {
        query.prepare(  "INSERT INTO public.produtos "
                        "(descricao, codigo_barra, id_unidade_medida, tipo_atividade, id_ncm, codigo_interno, id_anp) "
                        "VALUES (:descricao, :codigo_barra, :id_unidade_medida, :tipo_atividade::tipo_produto_atividade, :id_ncm, :codigo_interno, :id_anp) " "RETURNING id, uuid, criado_em, atualizado_em");
    } else {
        query.prepare(  "UPDATE public.produtos SET "
                        "descricao = :descricao, "
                        "codigo_barra = :codigo_barra, "
                        "id_unidade_medida = :id_unidade_medida, "
                        "tipo_atividade = :tipo_atividade::tipo_produto_atividade, "
                        "id_ncm = :id_ncm, "
                        "codigo_interno = :codigo_interno, "
                        "id_anp = :id_anp "
                        "WHERE id = :id "
                        "RETURNING atualizado_em");

        query.bindValue(":id", produto.id().value());
    }

    query.bindValue(":descricao", produto.descricao());
    query.bindValue(":codigo_barra", produto.codigoBarra().has_value() ? produto.codigoBarra().value() : QVariant());
    query.bindValue(":id_unidade_medida", produto.idUnidadeMedida());

    QString tipoAtividade = produto.tipoAtividade();
    if (tipoAtividade.isEmpty()) {
        tipoAtividade = TipoProdutoAtividade::MercadoriaRevenda;
    }
    query.bindValue(":tipo_atividade", tipoAtividade);

    query.bindValue(":id_ncm", produto.idNcm());

    query.bindValue(":codigo_interno", produto.codigoInterno().value());

    if (produto.idAnp().has_value() && produto.idAnp().value() > 0) {
        query.bindValue(":id_anp", produto.idAnp().value());
    } else {
        query.bindValue(":id_anp", QVariant(QMetaType::fromType<int>()));
    }

    if (query.exec() && query.next()) {
        produto.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        if (!produto.id().has_value()) {
            produto.setId(query.value("id").toInt());
            produto.setUuid(query.value("uuid").toUuid());
            produto.setCriadoEm(query.value("criado_em").toDateTime());
        }
        return true;
    } else {
        qCritical() << "[ProdutoRepository] Erro ao inserir/atualizar produto:" << query.lastError().text();

        return false;
    }
}

std::optional<Produto> ProdutoRepository::buscarPorId(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.produtos WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return Produto::fromRecord(query.record());
    }
    return std::nullopt;
}

std::optional<Produto> ProdutoRepository::buscarPorUuid(const QUuid &uuid)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.produtos WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);

    if (query.exec() && query.next()) {
        return Produto::fromRecord(query.record());
    }
    return std::nullopt;
}

std::optional<Produto> ProdutoRepository::buscarPorCodigoInterno(const QString &codigoInterno)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.produtos WHERE codigo_interno = :codigo_interno");
    query.bindValue(":codigo_interno", codigoInterno);

    if (query.exec() && query.next()) {
        return Produto::fromRecord(query.record());
    }
    return std::nullopt;
}

std::optional<Produto> ProdutoRepository::buscarPorCodigoBarra(const QString &codigoBarra)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.produtos WHERE codigo_barra = :codigo_barra");
    query.bindValue(":codigo_barra", codigoBarra);

    if (query.exec() && query.next()) {
        return Produto::fromRecord(query.record());
    }
    return std::nullopt;
}

std::optional<Produto> ProdutoRepository::buscarPorDescricao(const QString &descricao)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.produtos WHERE descricao = :descricao LIMIT 1");
    query.bindValue(":descricao", descricao);

    if (query.exec() && query.next()) {
        return Produto::fromRecord(query.record());
    }
    return std::nullopt;
}

QList<Produto> ProdutoRepository::listarTodos() {
    QList<Produto> lista;

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.produtos ORDER BY descricao ASC");

    if (query.exec()) {
        while (query.next()) {
            lista.append(Produto::fromRecord(query.record()));
        }
    } else {
        qCritical() << "[ProdutoRepository] Erro ao listar produtos:" << query.lastError().text();
    }

    return lista;
}

bool ProdutoRepository::deletar(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM public.produtos WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qCritical() << "[ProdutoRepository] Erro ao deletar produto id" << id << ":" << query.lastError().text();
        return false;
    }
    return true;
}
