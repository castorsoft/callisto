#include "NotaTerceiroItem.h"
#include <QVariant>

NotaTerceiroItem NotaTerceiroItem::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    NotaTerceiroItem item;

    if (record.contains(prefix + "id_item") && !record.isNull(prefix + "id_item"))
        item.setIdItem(record.value(prefix + "id_item").toInt());
    else if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        item.setIdItem(record.value(prefix + "id").toInt());

    if (record.contains(prefix + "id_documento_fiscal") && !record.isNull(prefix + "id_documento_fiscal"))
        item.setIdDocumentoFiscal(record.value(prefix + "id_documento_fiscal").toInt());

    if (record.contains(prefix + "numero_item") && !record.isNull(prefix + "numero_item"))
        item.setNumeroItem(record.value(prefix + "numero_item").toInt());

    if (record.contains(prefix + "codigo_produto") && !record.isNull(prefix + "codigo_produto"))
        item.setCodigoProduto(record.value(prefix + "codigo_produto").toString());

    if (record.contains(prefix + "codigo_barras") && !record.isNull(prefix + "codigo_barras"))
        item.setCodigoBarras(record.value(prefix + "codigo_barras").toString());

    if (record.contains(prefix + "descricao") && !record.isNull(prefix + "descricao"))
        item.setDescricao(record.value(prefix + "descricao").toString());

    if (record.contains(prefix + "unidade_medida") && !record.isNull(prefix + "unidade_medida"))
        item.setUnidadeMedida(record.value(prefix + "unidade_medida").toString());

    if (record.contains(prefix + "quantidade") && !record.isNull(prefix + "quantidade"))
        item.setQuantidade(record.value(prefix + "quantidade").toDouble());

    if (record.contains(prefix + "valor_unitario") && !record.isNull(prefix + "valor_unitario"))
        item.setValorUnitario(record.value(prefix + "valor_unitario").toDouble());

    if (record.contains(prefix + "valor_total") && !record.isNull(prefix + "valor_total"))
        item.setValorTotal(record.value(prefix + "valor_total").toDouble());

    if (record.contains(prefix + "ncm") && !record.isNull(prefix + "ncm"))
        item.setNcm(record.value(prefix + "ncm").toString());

    if (record.contains(prefix + "id_produto_fornecedor") && !record.isNull(prefix + "id_produto_fornecedor")) {
        item.setVinculado(true);
        item.setIdProdutoFornecedor(record.value(prefix + "id_produto_fornecedor").toInt());

        if (record.contains(prefix + "id_produto") && !record.isNull(prefix + "id_produto"))
            item.setIdProduto(record.value(prefix + "id_produto").toInt());

        if (record.contains(prefix + "multiplicador") && !record.isNull(prefix + "multiplicador"))
            item.setMultiplicador(record.value(prefix + "multiplicador").toDouble());

        if (record.contains(prefix + "id_unidade_interna") && !record.isNull(prefix + "id_unidade_interna"))
            item.setIdUnidadeMedidaInterna(record.value(prefix + "id_unidade_interna").toInt());
        else if (record.contains(prefix + "id_unidade_medida") && !record.isNull(prefix + "id_unidade_medida"))
            item.setIdUnidadeMedidaInterna(record.value(prefix + "id_unidade_medida").toInt());

        if (record.contains(prefix + "produto_codigo_interno") && !record.isNull(prefix + "produto_codigo_interno"))
            item.setProdutoCodigoInterno(record.value(prefix + "produto_codigo_interno").toString());

        if (record.contains(prefix + "produto_descricao") && !record.isNull(prefix + "produto_descricao"))
            item.setProdutoDescricao(record.value(prefix + "produto_descricao").toString());

        if (record.contains(prefix + "unidade_sigla") && !record.isNull(prefix + "unidade_sigla"))
            item.setSiglaUnidadeMedidaInterna(record.value(prefix + "unidade_sigla").toString());
    } else {
        item.setVinculado(false);
        item.setMultiplicador(1.0);
    }

    return item;
}

