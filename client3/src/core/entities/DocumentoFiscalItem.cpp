#include "DocumentoFiscalItem.h"
#include <QJsonValue>

DocumentoFiscalItem::DocumentoFiscalItem(int idDocumentoFiscal, int numeroItem, QString codigoProduto, QString descricao,
                                         QString ncm, std::optional<int> cfop, QString unidadeMedida, double quantidade,
                                         double valorUnitario, double valorTotal, double valorDesconto, QJsonObject dados)
    : m_idDocumentoFiscal(idDocumentoFiscal), m_numeroItem(numeroItem), m_codigoProduto(codigoProduto),
      m_descricao(descricao), m_ncm(ncm), m_cfop(cfop), m_unidadeMedida(unidadeMedida), m_quantidade(quantidade),
      m_valorUnitario(valorUnitario), m_valorTotal(valorTotal), m_valorDesconto(valorDesconto), m_dados(dados)
{
}

DocumentoFiscalItem::DocumentoFiscalItem(int id, int idDocumentoFiscal, int numeroItem, QString codigoProduto,
                                         QString descricao, QString ncm, std::optional<int> cfop, QString unidadeMedida,
                                         double quantidade, double valorUnitario, double valorTotal, double valorDesconto,
                                         QJsonObject dados, QDateTime criadoEm, QDateTime atualizadoEm)
    : m_id(id), m_idDocumentoFiscal(idDocumentoFiscal), m_numeroItem(numeroItem), m_codigoProduto(codigoProduto),
      m_descricao(descricao), m_ncm(ncm), m_cfop(cfop), m_unidadeMedida(unidadeMedida), m_quantidade(quantidade),
      m_valorUnitario(valorUnitario), m_valorTotal(valorTotal), m_valorDesconto(valorDesconto), m_dados(dados),
      m_criadoEm(criadoEm), m_atualizadoEm(atualizadoEm)
{
}

DocumentoFiscalItem DocumentoFiscalItem::loadFromDatabase(int id, int idDocumentoFiscal, int numeroItem,
                                                          QString codigoProduto, QString descricao, QString ncm,
                                                          std::optional<int> cfop, QString unidadeMedida,
                                                          double quantidade, double valorUnitario, double valorTotal,
                                                          double valorDesconto, QJsonObject dados,
                                                          QDateTime criadoEm, QDateTime atualizadoEm)
{
    return DocumentoFiscalItem(id, idDocumentoFiscal, numeroItem, codigoProduto, descricao, ncm, cfop,
                               unidadeMedida, quantidade, valorUnitario, valorTotal, valorDesconto, dados,
                               criadoEm, atualizadoEm);
}

void DocumentoFiscalItem::hidratar(int id, QDateTime criadoEm, QDateTime atualizadoEm)
{
    m_id = id;
    m_criadoEm = criadoEm;
    m_atualizadoEm = atualizadoEm;
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
