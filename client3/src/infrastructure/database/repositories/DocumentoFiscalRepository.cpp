#include "DocumentoFiscalRepository.h"
#include "../DatabaseManager.h"
#include <QVariant>
#include <QJsonDocument>
#include <QDebug>

DocumentoFiscalRepository::DocumentoFiscalRepository()
{
}

bool DocumentoFiscalRepository::salvar(DocumentoFiscal& documentoFiscal)
{
    auto db = DatabaseManager::instance().database();

    if (!db.transaction()) {
        qCritical() << "[DocumentoFiscalRepository] Erro ao iniciar a transacao:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);

    if (!documentoFiscal.id().has_value()) {
        query.prepare(  "INSERT INTO public.documentos_fiscais "
                        "(chave_acesso, tipo_operacao, tipo_emissao, id_pessoa, modelo, serie, numero, dh_emissao, dados) "
                        "VALUES (:chave_acesso, :tipo_operacao::tipo_dfe_operacao, :tipo_emissao::tipo_dfe_emissao, :id_pessoa, :modelo, :serie, :numero, :dh_emissao, :dados::jsonb)"
                        "RETURNING id, criado_em, atualizado_em");
    } else {
        query.prepare(  "UPDATE public.documentos_fiscais SET"
                        "chave_acesso = :chave_acesso, "
                        "tipo_operacao = :tipo_operacao::tipo_dfe_operacao, "
                        "tipo_emissao = :tipo_emissao::tipo_dfe_emissao, "
                        "id_pessoa = :id_pessoa, "
                        "modelo = :modelo, "
                        "serie = :serie, "
                        "numero = :numero, "
                        "dh_emissao = :dh_emissao, "
                        "dados = :dados::jsonb "
                        "WHERE id = :id "
                        "RETURNING atualizado_em");

        query.bindValue(":id", documentoFiscal.id().value());
    }

    query.bindValue(":chave_acesso", documentoFiscal.chaveAcesso());
    query.bindValue(":tipo_operacao", documentoFiscal.tipoOperacao());
    query.bindValue(":tipo_emissao", documentoFiscal.tipoEmissao());
    query.bindValue(":id_pessoa", documentoFiscal.idPessoa().value());
    query.bindValue(":modelo", documentoFiscal.modelo());

    if (documentoFiscal.serie().isEmpty()) {
        query.bindValue(":serie", QVariant(QMetaType::fromType<QString>())); // NULL
    } else {
        query.bindValue(":serie", documentoFiscal.serie());
    }

    query.bindValue(":numero", documentoFiscal.numero());
    query.bindValue(":dh_emissao", documentoFiscal.dhEmissao());

    if (documentoFiscal.dados().isEmpty()) {
        query.bindValue(":dados", QVariant(QMetaType::fromType<QString>()));
    } else {
        QJsonDocument doc(documentoFiscal.dados());
        query.bindValue(":dados", QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    }

    if (query.exec() && query.next()) {
        int id = query.value("id").toInt();
        documentoFiscal.setAtualizadoEm(query.value("atualizado_em").toDateTime());

        if (!documentoFiscal.id().has_value()) {
            documentoFiscal.setId(id);
            documentoFiscal.setCriadoEm(query.value("criado_em").toDateTime());
        }

        if (!inserirItens(documentoFiscal.itens(), id, db)) {
            db.rollback();
            return false;
        }

        if (!db.commit()) {
            qCritical() << "[DocumentoFiscalRepository] Erro ao commitar transacao:" << db.lastError().text();
            db.rollback();
            return false;
        }

        return true;
    } else{
        qCritical() << "[DocumentoFiscalRepository] Erro ao inserir/atualizar documento fiscal:" << query.lastError().text();
        db.rollback();

        return false;
    }
}

bool DocumentoFiscalRepository::inserirItens(const std::vector<DocumentoFiscalItem>& itens, int idDocumentoFiscal, QSqlDatabase& db)
{
    if (itens.empty()) return true;

    for (auto item : itens) {
        QSqlQuery query(db);

        if (!item.id().has_value()) {
            query.prepare(  "INSERT INTO public.documentos_fiscais_itens "
                            "(id_documento_fiscal, numero_item, codigo_produto, descricao, unidade_medida, quantidade, valor_unitario, valor_total, valor_desconto, dados) "
                            "VALUES (:id_documento_fiscal, :numero_item, :codigo_produto, :descricao, :unidade_medida, :quantidade, :valor_unitario, :valor_total, :valor_desconto, :dados::jsonb) "
                            "RETURNING id, criado_em, atualizado_em");
        } else {
            query.prepare(  "UPDATE public.documentos_fiscais_itens SET "
                            "id_documento_fiscal = :id_documento_fiscal, "
                            "numero_item = :numero_item, "
                            "codigo_produto = :codigo_produto, "
                            "descricao = :descricao, "
                            "unidade_medida = :unidade_medida, "
                            "quantidade = :quantidade, "
                            "valor_unitario = :valor_unitario, "
                            "valor_total = :valor_total, "
                            "valor_desconto = :valor_desconto, "
                            "dados = :dados::jsonb "
                            "WHERE id = :id "
                            "RETURNING atualizado_em");
            query.bindValue(":id", item.id().value());
        }

        query.bindValue(":id_documento_fiscal", idDocumentoFiscal);
        query.bindValue(":numero_item", item.numeroItem());
        query.bindValue(":codigo_produto", item.codigoProduto());
        query.bindValue(":descricao", item.descricao());

        query.bindValue(":unidade_medida", item.unidadeMedida().isEmpty() ? QVariant(QMetaType::fromType<QString>()) : item.unidadeMedida());

        query.bindValue(":quantidade", item.quantidade());
        query.bindValue(":valor_unitario", item.valorUnitario());
        query.bindValue(":valor_total", item.valorTotal());
        query.bindValue(":valor_desconto", item.valorDesconto());

        if (item.dados().isEmpty()) {
            query.bindValue(":dados", QVariant(QMetaType::fromType<QString>()));
        } else {
            QJsonDocument docDados(item.dados());
            query.bindValue(":dados", QString::fromUtf8(docDados.toJson(QJsonDocument::Compact)));
        }

        if (!query.exec()) {
            qCritical() << "[DocumentoFiscalRepository] Erro ao inserir/atualizar item do documento fiscal:" << query.lastError().text();
            return false;
        }

        if (query.exec() && query.next()) {
            int idItem = query.value("id").toInt();
            item.setAtualizadoEm(query.value("atualizado_em").toDateTime());
            if (!item.id().has_value()) {
                item.setId(idItem);
                item.setCriadoEm(query.value("criado_em").toDateTime());
            }

            if (!inserirTributacoesItem(item, idItem, db)) {
                return false;
            }
        }
    }
    return true;
}

bool DocumentoFiscalRepository::inserirTributacoesItem(const DocumentoFiscalItem& item, int idItem, QSqlDatabase& db)
{
    for (auto tributo: item.tributos()) {
        QSqlQuery query(db);

        if (!tributo.id().has_value()) {
            query.prepare(  "INSERT INTO public.documentos_fiscais_itens_tributacoes "
                            "(id_item, tipo_tributo, dados) "
                            "VALUES (:id_item, :tipo_tributo::tipo_produto_tributacao, :dados::jsonb) "
                            "RETURNING id, criado_em, atualizado_em");
        } else {
            query.prepare(  "UPDATE public.documentos_fiscais_itens_tributacoes SET "
                            "id_item = :id_item, "
                            "tipo_tributo = :tipo_tributo::tipo_produto_tributacao, "
                            "dados = :dados::jsonb "
                            "WHERE id = :id "
                            "RETURNING atualizado_em");

            query.bindValue(":id", tributo.id().value());
        }

        query.bindValue(":id_item", idItem);
        query.bindValue(":tipo_tributo", tributo.tipoTributo());

        if (tributo.dados().isEmpty()) {
            query.bindValue(":dados", QVariant(QMetaType::fromType<QString>()));
        } else {
            QJsonDocument docDados(tributo.dados());
            query.bindValue(":dados", QString::fromUtf8(docDados.toJson(QJsonDocument::Compact)));
        }

        if (query.exec() && query.next()) {
            tributo.setAtualizadoEm(query.value("atualizado_em").toDateTime());
            if (!tributo.id().has_value())             {
                tributo.setId(query.value("id").toInt());
                tributo.setCriadoEm(query.value("criado_em").toDateTime());
            }
        } else {
            qCritical() << "[DocumentoFiscalRepository] Erro ao inserir/atualizar tributacao (" << tributo.tipoTributo() << "):" << query.lastError().text();

            return false;
        }
    }
    return true;
}

std::optional<DocumentoFiscal> DocumentoFiscalRepository::buscarPorId(int id, bool carregarItens)
{
    // TODO: Implementar busca por ID
    return std::nullopt;
}

std::optional<DocumentoFiscal> DocumentoFiscalRepository::buscarPorChave(const QString& chaveAcesso, bool carregarItens)
{
    // TODO: Implementar busca por chave
    return std::nullopt;
}

bool DocumentoFiscalRepository::existeDocumento(const QString& chaveAcesso)
{
    auto db = DatabaseManager::instance().database();
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("SELECT 1 FROM public.documentos_fiscais WHERE chave_acesso = :chave_acesso LIMIT 1");
    query.bindValue(":chave_acesso", chaveAcesso);

    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}
