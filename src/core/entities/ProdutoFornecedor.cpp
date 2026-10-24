#include "ProdutoFornecedor.h"
#include <QVariant>

ProdutoFornecedor::ProdutoFornecedor(int idProduto,
                                     int idFornecedor,
                                     const QString &codigoFornecedor,
                                     int idUnidadeMedida,
                                     double multiplicador,
                                     const std::optional<QString> &codigoBarraCaixa)
    : m_idProduto(idProduto)
    , m_idFornecedor(idFornecedor)
    , m_codigoFornecedor(codigoFornecedor)
    , m_idUnidadeMedida(idUnidadeMedida)
    , m_multiplicador(multiplicador)
    , m_codigoBarraCaixa(codigoBarraCaixa)
{
}

ProdutoFornecedor ProdutoFornecedor::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    ProdutoFornecedor pf;

    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        pf.setId(record.value(prefix + "id").toInt());

    if (record.contains(prefix + "uuid") && !record.isNull(prefix + "uuid"))
        pf.setUuid(record.value(prefix + "uuid").toUuid());

    if (record.contains(prefix + "id_produto") && !record.isNull(prefix + "id_produto"))
        pf.setIdProduto(record.value(prefix + "id_produto").toInt());

    if (record.contains(prefix + "id_fornecedor") && !record.isNull(prefix + "id_fornecedor"))
        pf.setIdFornecedor(record.value(prefix + "id_fornecedor").toInt());

    if (record.contains(prefix + "codigo_fornecedor") && !record.isNull(prefix + "codigo_fornecedor"))
        pf.setCodigoFornecedor(record.value(prefix + "codigo_fornecedor").toString());

    if (record.contains(prefix + "id_unidade_medida") && !record.isNull(prefix + "id_unidade_medida"))
        pf.setIdUnidadeMedida(record.value(prefix + "id_unidade_medida").toInt());

    if (record.contains(prefix + "multiplicador") && !record.isNull(prefix + "multiplicador"))
        pf.setMultiplicador(record.value(prefix + "multiplicador").toDouble());

    if (record.contains(prefix + "codigo_barra_caixa") && !record.isNull(prefix + "codigo_barra_caixa"))
        pf.setCodigoBarraCaixa(record.value(prefix + "codigo_barra_caixa").toString());

    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        pf.setCriadoEm(record.value(prefix + "criado_em").toDateTime());

    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        pf.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return pf;
}

