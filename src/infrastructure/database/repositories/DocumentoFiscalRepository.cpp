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
                            "(id_documento_fiscal, numero_item, codigo_produto, codigo_barras, descricao, unidade_medida, quantidade, valor_unitario, valor_total, valor_desconto, dados) "
                            "VALUES (:id_documento_fiscal, :numero_item, :codigo_produto, :codigo_barras, :descricao, :unidade_medida, :quantidade, :valor_unitario, :valor_total, :valor_desconto, :dados::jsonb) "
                            "RETURNING id, criado_em, atualizado_em");
        } else {
            query.prepare(  "UPDATE public.documentos_fiscais_itens SET "
                            "id_documento_fiscal = :id_documento_fiscal, "
                            "numero_item = :numero_item, "
                            "codigo_produto = :codigo_produto, "
                            "codigo_barras = :codigo_barras, "
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
        query.bindValue(":codigo_barras", item.codigoBarras().isEmpty() ? QVariant(QMetaType::fromType<QString>()) : item.codigoBarras());
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

        if (query.next()) {
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

QList<DocumentoFiscal> DocumentoFiscalRepository::listarPorPeriodo(const QDate &dataInicio, const QDate &dataFim, bool carregarItens)
{
    QList<DocumentoFiscal> lista;
    auto db = DatabaseManager::instance().database();
    if (!db.isOpen()) return lista;

    QSqlQuery query(db);
    query.prepare(
        "SELECT id, chave_acesso, tipo_operacao, tipo_emissao, id_pessoa, modelo, serie, numero, dh_emissao, dados, criado_em, atualizado_em "
        "FROM public.documentos_fiscais "
        "WHERE dh_emissao::date >= :data_inicio AND dh_emissao::date <= :data_fim "
        "ORDER BY dh_emissao ASC, id ASC"
    );
    query.bindValue(":data_inicio", dataInicio);
    query.bindValue(":data_fim", dataFim);

    if (query.exec()) {
        while (query.next()) {
            DocumentoFiscal doc = DocumentoFiscal::fromRecord(query.record());
            if (carregarItens && doc.id().has_value()) {
                QSqlQuery itemQuery(db);
                itemQuery.prepare(
                    "SELECT id, id_documento_fiscal, numero_item, codigo_produto, codigo_barras, descricao, unidade_medida, "
                    "quantidade, valor_unitario, valor_total, valor_desconto, dados, criado_em, atualizado_em "
                    "FROM public.documentos_fiscais_itens "
                    "WHERE id_documento_fiscal = :id_doc "
                    "ORDER BY numero_item ASC"
                );
                itemQuery.bindValue(":id_doc", doc.id().value());
                if (itemQuery.exec()) {
                    std::vector<DocumentoFiscalItem> itens;
                    while (itemQuery.next()) {
                        itens.push_back(DocumentoFiscalItem::fromRecord(itemQuery.record()));
                    }
                    doc.setItens(itens);
                }
            }
            lista.append(doc);
        }
    } else {
        qCritical() << "[DocumentoFiscalRepository] Erro ao listar documentos por periodo:" << query.lastError().text();
    }
    return lista;
}

QList<NotaTerceiroResumo> DocumentoFiscalRepository::listarNotasTerceirosResumo(bool apenasPendentes)
{
    QList<NotaTerceiroResumo> lista;
    auto db = DatabaseManager::instance().database();
    if (!db.isOpen()) return lista;

    QSqlQuery query(db);
    QString sql =
        "SELECT "
        "    df.id, "
        "    df.chave_acesso, "
        "    df.modelo, "
        "    df.serie, "
        "    df.numero, "
        "    df.dh_emissao AS data_emissao, "
        "    df.id_pessoa, "
        "    p.razao_social AS fornecedor_nome, "
        "    p.cnpj_cpf AS fornecedor_cnpj, "
        "    COUNT(dfi.id) AS total_itens, "
        "    COUNT(dfi.id) FILTER (WHERE pf.id IS NULL) AS itens_pendentes "
        "FROM public.documentos_fiscais df "
        "INNER JOIN public.pessoas p ON p.id = df.id_pessoa "
        "LEFT JOIN public.documentos_fiscais_itens dfi ON dfi.id_documento_fiscal = df.id "
        "LEFT JOIN public.produtos_fornecedores pf "
        "    ON pf.id_fornecedor = df.id_pessoa "
        "    AND pf.codigo_fornecedor = dfi.codigo_produto "
        "WHERE df.tipo_emissao = 'terceiro' "
        "GROUP BY df.id, df.chave_acesso, df.modelo, df.serie, df.numero, df.dh_emissao, df.id_pessoa, p.razao_social, p.cnpj_cpf ";

    if (apenasPendentes) {
        sql += "HAVING COUNT(dfi.id) FILTER (WHERE pf.id IS NULL) > 0 ";
    }

    sql += "ORDER BY df.dh_emissao DESC, df.id DESC";

    query.prepare(sql);

    if (query.exec()) {
        while (query.next()) {
            lista.append(NotaTerceiroResumo::fromRecord(query.record()));
        }
    } else {
        qCritical() << "[DocumentoFiscalRepository] Erro ao listar notas de terceiros:" << query.lastError().text();
    }

    return lista;
}

QList<NotaTerceiroItem> DocumentoFiscalRepository::carregarItensNotaTerceiro(int idDocumentoFiscal)
{
    QList<NotaTerceiroItem> lista;
    auto db = DatabaseManager::instance().database();
    if (!db.isOpen()) return lista;

    QSqlQuery query(db);
    query.prepare(
        "SELECT "
        "    dfi.id AS id_item, "
        "    dfi.id_documento_fiscal, "
        "    dfi.numero_item, "
        "    dfi.codigo_produto, "
        "    dfi.codigo_barras, "
        "    dfi.descricao, "
        "    dfi.unidade_medida, "
        "    dfi.quantidade, "
        "    dfi.valor_unitario, "
        "    dfi.valor_total, "
        "    dfi.dados->>'ncm' AS ncm, "
        "    pf.id AS id_produto_fornecedor, "
        "    pf.id_produto, "
        "    pf.multiplicador, "
        "    pf.id_unidade_medida AS id_unidade_interna, "
        "    prod.codigo_interno AS produto_codigo_interno, "
        "    prod.descricao AS produto_descricao, "
        "    um.sigla AS unidade_sigla "
        "FROM public.documentos_fiscais_itens dfi "
        "INNER JOIN public.documentos_fiscais df ON df.id = dfi.id_documento_fiscal "
        "LEFT JOIN public.produtos_fornecedores pf "
        "    ON pf.id_fornecedor = df.id_pessoa "
        "    AND pf.codigo_fornecedor = dfi.codigo_produto "
        "LEFT JOIN public.produtos prod ON prod.id = pf.id_produto "
        "LEFT JOIN public.unidades_medida um ON um.id = pf.id_unidade_medida "
        "WHERE dfi.id_documento_fiscal = :id_documento_fiscal "
        "ORDER BY dfi.numero_item ASC"
    );
    query.bindValue(":id_documento_fiscal", idDocumentoFiscal);

    if (query.exec()) {
        while (query.next()) {
            lista.append(NotaTerceiroItem::fromRecord(query.record()));
        }
    } else {
        qCritical() << "[DocumentoFiscalRepository] Erro ao carregar itens da nota de terceiro:" << query.lastError().text();
    }

    return lista;
}
