#include "ProdutoFornecedorRepository.h"
#include "../DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool ProdutoFornecedorRepository::salvar(ProdutoFornecedor &pf)
{
    QSqlQuery query(DatabaseManager::instance().database());

    if (!pf.id().has_value() || pf.id().value() == -1) {
        query.prepare(
            "INSERT INTO public.produtos_fornecedores ("
            "  id_produto, id_fornecedor, codigo_fornecedor, id_unidade_medida, multiplicador, codigo_barra_caixa"
            ") VALUES ("
            "  :id_produto, :id_fornecedor, :codigo_fornecedor, :id_unidade_medida, :multiplicador, :codigo_barra_caixa"
            ") ON CONFLICT (id_fornecedor, codigo_fornecedor) DO UPDATE SET "
            "  id_produto = EXCLUDED.id_produto, "
            "  id_unidade_medida = EXCLUDED.id_unidade_medida, "
            "  multiplicador = EXCLUDED.multiplicador, "
            "  codigo_barra_caixa = EXCLUDED.codigo_barra_caixa, "
            "  atualizado_em = CURRENT_TIMESTAMP "
            "RETURNING id, uuid, criado_em, atualizado_em"
        );
    } else {
        query.prepare(
            "UPDATE public.produtos_fornecedores SET "
            "  id_produto = :id_produto, "
            "  id_fornecedor = :id_fornecedor, "
            "  codigo_fornecedor = :codigo_fornecedor, "
            "  id_unidade_medida = :id_unidade_medida, "
            "  multiplicador = :multiplicador, "
            "  codigo_barra_caixa = :codigo_barra_caixa, "
            "  atualizado_em = CURRENT_TIMESTAMP "
            "WHERE id = :id "
            "RETURNING atualizado_em"
        );
        query.bindValue(":id", pf.id().value());
    }

    query.bindValue(":id_produto", pf.idProduto());
    query.bindValue(":id_fornecedor", pf.idFornecedor());
    query.bindValue(":codigo_fornecedor", pf.codigoFornecedor());
    query.bindValue(":id_unidade_medida", pf.idUnidadeMedida());
    query.bindValue(":multiplicador", pf.multiplicador());
    query.bindValue(":codigo_barra_caixa", pf.codigoBarraCaixa().has_value() ? pf.codigoBarraCaixa().value() : QVariant());

    if (query.exec() && query.next()) {
        pf.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        if (!pf.id().has_value() || pf.id().value() == -1) {
            pf.setId(query.value("id").toInt());
            pf.setUuid(query.value("uuid").toUuid());
            pf.setCriadoEm(query.value("criado_em").toDateTime());
        }
        return true;
    } else {
        qCritical() << "[ProdutoFornecedorRepository] Erro ao salvar produto_fornecedor:" << query.lastError().text();
        return false;
    }
}

std::optional<ProdutoFornecedor> ProdutoFornecedorRepository::buscarPorId(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.produtos_fornecedores WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return ProdutoFornecedor::fromRecord(query.record());
    }
    return std::nullopt;
}

std::optional<ProdutoFornecedor> ProdutoFornecedorRepository::buscarPorFornecedorECodigo(int idFornecedor, const QString &codigoFornecedor)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.produtos_fornecedores WHERE id_fornecedor = :id_fornecedor AND codigo_fornecedor = :codigo_fornecedor LIMIT 1");
    query.bindValue(":id_fornecedor", idFornecedor);
    query.bindValue(":codigo_fornecedor", codigoFornecedor);

    if (query.exec() && query.next()) {
        return ProdutoFornecedor::fromRecord(query.record());
    }
    return std::nullopt;
}

QList<ProdutoFornecedor> ProdutoFornecedorRepository::listarPorFornecedor(int idFornecedor)
{
    QList<ProdutoFornecedor> lista;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.produtos_fornecedores WHERE id_fornecedor = :id_fornecedor ORDER BY codigo_fornecedor ASC");
    query.bindValue(":id_fornecedor", idFornecedor);

    if (query.exec()) {
        while (query.next()) {
            lista.append(ProdutoFornecedor::fromRecord(query.record()));
        }
    } else {
        qCritical() << "[ProdutoFornecedorRepository] Erro ao listar por fornecedor:" << query.lastError().text();
    }
    return lista;
}

QList<ProdutoFornecedor> ProdutoFornecedorRepository::listarPorProduto(int idProduto)
{
    QList<ProdutoFornecedor> lista;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM public.produtos_fornecedores WHERE id_produto = :id_produto ORDER BY id ASC");
    query.bindValue(":id_produto", idProduto);

    if (query.exec()) {
        while (query.next()) {
            lista.append(ProdutoFornecedor::fromRecord(query.record()));
        }
    } else {
        qCritical() << "[ProdutoFornecedorRepository] Erro ao listar por produto:" << query.lastError().text();
    }
    return lista;
}

bool ProdutoFornecedorRepository::deletar(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM public.produtos_fornecedores WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

