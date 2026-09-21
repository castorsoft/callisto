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
    if (!db.isOpen()) {
        qCritical() << "[DocumentoFiscalRepository] Falha ao salvar: Banco de dados nao esta aberto.";
        return false;
    }

    if (!db.transaction()) {
        qCritical() << "[DocumentoFiscalRepository] Falha ao iniciar transacao:" << db.lastError().text();
        return false;
    }

    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO public.documentos_fiscais (
            chave_acesso, tipo_operacao, tipo_emissao, id_pessoa,
            modelo, serie, numero, dh_emissao, dados
        ) VALUES (
            :chave_acesso, :tipo_operacao::tipo_dfe_operacao, :tipo_emissao::tipo_dfe_emissao, :id_pessoa,
            :modelo, :serie, :numero, :dh_emissao, :dados
        ) RETURNING id, criado_em, atualizado_em
    )");

    query.bindValue(":chave_acesso", documentoFiscal.chaveAcesso());
    // Converter enum de tipo_operacao se necessario, assumindo string do C++ e mapeamento correto
    query.bindValue(":tipo_operacao", documentoFiscal.tipoOperacao());
    query.bindValue(":tipo_emissao", documentoFiscal.tipoEmissao());
    query.bindValue(":id_pessoa", documentoFiscal.idPessoa());
    query.bindValue(":modelo", documentoFiscal.modelo());

    if (documentoFiscal.serie().isEmpty()) {
        query.bindValue(":serie", QVariant(QMetaType::fromType<QString>())); // NULL
    } else {
        query.bindValue(":serie", documentoFiscal.serie());
    }

    query.bindValue(":numero", documentoFiscal.numero());
    query.bindValue(":dh_emissao", documentoFiscal.dhEmissao());

    QJsonDocument doc(documentoFiscal.dados());
    query.bindValue(":dados", doc.toJson(QJsonDocument::Compact));

    if (!query.exec()) {
        qCritical() << "[DocumentoFiscalRepository] Erro ao inserir documento fiscal:" << query.lastError().text();
        db.rollback();
        return false;
    }

    if (query.next()) {
        int id = query.value("id").toInt();
        QDateTime criadoEm = query.value("criado_em").toDateTime();
        QDateTime atualizadoEm = query.value("atualizado_em").toDateTime();

        documentoFiscal.hidratar(id, criadoEm, atualizadoEm);

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
    }

    db.rollback();
    return false;
}

bool DocumentoFiscalRepository::inserirItens(const std::vector<DocumentoFiscalItem>& itens, int idDocumentoFiscal, QSqlDatabase& db)
{
    if (itens.empty()) return true;

    QSqlQuery query(db);
    query.prepare(R"(
        INSERT INTO public.documentos_fiscais_itens (
            id_documento_fiscal, numero_item, codigo_produto, descricao, ncm,
            cfop, unidade_medida, quantidade, valor_unitario, valor_total, valor_desconto, dados
        ) VALUES (
            :id_documento_fiscal, :numero_item, :codigo_produto, :descricao, :ncm,
            :cfop, :unidade_medida, :quantidade, :valor_unitario, :valor_total, :valor_desconto, :dados
        ) RETURNING id, criado_em, atualizado_em
    )");

    for (const auto& item : itens) {
        query.bindValue(":id_documento_fiscal", idDocumentoFiscal);
        query.bindValue(":numero_item", item.numeroItem());
        query.bindValue(":codigo_produto", item.codigoProduto());
        query.bindValue(":descricao", item.descricao());

        query.bindValue(":ncm", item.ncm().isEmpty() ? QVariant(QMetaType::fromType<QString>()) : item.ncm());

        if (item.cfop().has_value()) {
            query.bindValue(":cfop", item.cfop().value());
        } else {
            query.bindValue(":cfop", QVariant(QMetaType::fromType<int>())); // NULL
        }

        query.bindValue(":unidade_medida", item.unidadeMedida().isEmpty() ? QVariant(QMetaType::fromType<QString>()) : item.unidadeMedida());

        query.bindValue(":quantidade", item.quantidade());
        query.bindValue(":valor_unitario", item.valorUnitario());
        query.bindValue(":valor_total", item.valorTotal());
        query.bindValue(":valor_desconto", item.valorDesconto());

        QJsonDocument docDados(item.dados());
        query.bindValue(":dados", docDados.toJson(QJsonDocument::Compact));

        if (!query.exec()) {
            qCritical() << "[DocumentoFiscalRepository] Erro ao inserir item do documento fiscal:" << query.lastError().text();
            return false;
        }

        if (query.next()) {
            int idItem = query.value("id").toInt();

            // Aqui devemos inserir os impostos para o item inserido
            if (!inserirTributacoesItem(item, idItem, db)) {
                return false;
            }
        }
    }
    return true;
}

bool DocumentoFiscalRepository::inserirTributacoesItem(const DocumentoFiscalItem& item, int idItem, QSqlDatabase& db)
{
    // Funcao auxiliar para inserir um imposto generico
    auto inserirTributo = [&](const QString& tipoTributo, const QString& nomeObjetoJson) -> bool {
        if (item.dados().contains("impostos") && item.dados()["impostos"].isObject()) {
            auto impostos = item.dados()["impostos"].toObject();
            if (impostos.contains(nomeObjetoJson) && impostos[nomeObjetoJson].isObject()) {
                QSqlQuery query(db);
                query.prepare(R"(
                    INSERT INTO public.documentos_fiscais_itens_tributacoes (
                        id_item, tipo_tributo, dados
                    ) VALUES (
                        :id_item, :tipo_tributo::tipo_produto_tributacao, :dados
                    )
                )");

                query.bindValue(":id_item", idItem);
                query.bindValue(":tipo_tributo", tipoTributo); // ex: 'icms', 'pis', 'cofins', 'ipi'

                QJsonDocument docTributo(impostos[nomeObjetoJson].toObject());
                query.bindValue(":dados", docTributo.toJson(QJsonDocument::Compact));

                if (!query.exec()) {
                    qCritical() << "[DocumentoFiscalRepository] Erro ao inserir tributacao (" << tipoTributo << "):" << query.lastError().text();
                    return false;
                }
            }
        }
        return true;
    };

    if (!inserirTributo("icms", "icms")) return false;
    if (!inserirTributo("pis", "pis")) return false;
    if (!inserirTributo("cofins", "cofins")) return false;
    if (!inserirTributo("ipi", "ipi")) return false;

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
