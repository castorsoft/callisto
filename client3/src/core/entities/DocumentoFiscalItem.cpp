#include "DocumentoFiscalItem.h"
#include <QJsonValue>

DocumentoFiscalItem::DocumentoFiscalItem(int idDocumentoFiscal, int numeroItem, QString codigoProduto, QString descricao,
                                         QString unidadeMedida, double quantidade,
                                         double valorUnitario, double valorTotal, double valorDesconto, QJsonObject dados)
    : m_idDocumentoFiscal(idDocumentoFiscal), m_numeroItem(numeroItem), m_codigoProduto(codigoProduto),
      m_descricao(descricao), m_unidadeMedida(unidadeMedida), m_quantidade(quantidade),
      m_valorUnitario(valorUnitario), m_valorTotal(valorTotal), m_valorDesconto(valorDesconto), m_dados(dados)
{
}

DocumentoFiscalItem DocumentoFiscalItem::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    DocumentoFiscalItem dfi;

    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        dfi.setId(record.value(prefix + "id").toInt());

    if (record.contains(prefix + "id_documento_fiscal") && !record.isNull(prefix + "id_documento_fiscal"))
        dfi.m_idDocumentoFiscal = record.value(prefix + "id_documento_fiscal").toInt();

    if (record.contains(prefix + "numero_item") && !record.isNull(prefix + "numero_item"))
        dfi.m_numeroItem = record.value(prefix + "numero_item").toInt();

    if (record.contains(prefix + "codigo_produto") && !record.isNull(prefix + "codigo_produto"))
        dfi.m_codigoProduto = record.value(prefix + "codigo_produto").toString();

    if (record.contains(prefix + "descricao") && !record.isNull(prefix + "descricao"))
        dfi.m_descricao = record.value(prefix + "descricao").toString();

    if (record.contains(prefix + "unidade_medida") && !record.isNull(prefix + "unidade_medida"))
        dfi.m_unidadeMedida = record.value(prefix + "unidade_medida").toString();

    if (record.contains(prefix + "quantidade") && !record.isNull(prefix + "quantidade"))
        dfi.m_quantidade = record.value(prefix + "quantidade").toDouble();

    if (record.contains(prefix + "valor_unitario") && !record.isNull(prefix + "valor_unitario"))
        dfi.m_valorUnitario = record.value(prefix + "valor_unitario").toDouble();

    if (record.contains(prefix + "valor_total") && !record.isNull(prefix + "valor_total"))
        dfi.m_valorTotal = record.value(prefix + "valor_total").toDouble();

    if (record.contains(prefix + "valor_desconto") && !record.isNull(prefix + "valor_desconto"))
        dfi.m_valorDesconto = record.value(prefix + "valor_desconto").toDouble();

    if (record.contains(prefix + "dados") && !record.isNull(prefix + "dados"))
        dfi.m_dados = record.value(prefix + "dados").toJsonObject();

    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        dfi.setCriadoEm(record.value(prefix + "criado_em").toDateTime());

    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        dfi.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return dfi;
}

QString DocumentoFiscalItem::cstIcms() const
{
    if (m_dados.contains("impostos") && m_dados["impostos"].isObject()) {
        auto impostos = m_dados["impostos"].toObject();
        if (impostos.contains("icms") && impostos["icms"].isObject()) {
            auto icms = impostos["icms"].toObject();
            if (icms.contains("cst")) return icms["cst"].toString();
            if (icms.contains("csosn")) return icms["csosn"].toString();
        }
    }
    return QString();
}

QString DocumentoFiscalItem::cstPis() const
{
    if (m_dados.contains("impostos") && m_dados["impostos"].isObject()) {
        auto impostos = m_dados["impostos"].toObject();
        if (impostos.contains("pis") && impostos["pis"].isObject()) {
            return impostos["pis"].toObject()["cst"].toString();
        }
    }
    return QString();
}

QString DocumentoFiscalItem::cstCofins() const
{
    if (m_dados.contains("impostos") && m_dados["impostos"].isObject()) {
        auto impostos = m_dados["impostos"].toObject();
        if (impostos.contains("cofins") && impostos["cofins"].isObject()) {
            return impostos["cofins"].toObject()["cst"].toString();
        }
    }
    return QString();
}

QString DocumentoFiscalItem::cstIpi() const
{
    if (m_dados.contains("impostos") && m_dados["impostos"].isObject()) {
        auto impostos = m_dados["impostos"].toObject();
        if (impostos.contains("ipi") && impostos["ipi"].isObject()) {
            return impostos["ipi"].toObject()["cst"].toString();
        }
    }
    return QString();
}

double DocumentoFiscalItem::baseIcms() const
{
    if (m_dados.contains("impostos") && m_dados["impostos"].isObject()) {
        auto impostos = m_dados["impostos"].toObject();
        if (impostos.contains("icms") && impostos["icms"].isObject()) {
            return impostos["icms"].toObject()["vBC"].toDouble(0.0);
        }
    }
    return 0.0;
}

double DocumentoFiscalItem::valorIcms() const
{
    if (m_dados.contains("impostos") && m_dados["impostos"].isObject()) {
        auto impostos = m_dados["impostos"].toObject();
        if (impostos.contains("icms") && impostos["icms"].isObject()) {
            return impostos["icms"].toObject()["vICMS"].toDouble(0.0);
        }
    }
    return 0.0;
}

double DocumentoFiscalItem::aliquotaIcms() const
{
    if (m_dados.contains("impostos") && m_dados["impostos"].isObject()) {
        auto impostos = m_dados["impostos"].toObject();
        if (impostos.contains("icms") && impostos["icms"].isObject()) {
            return impostos["icms"].toObject()["pICMS"].toDouble(0.0);
        }
    }
    return 0.0;
}
