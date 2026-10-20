#include "NotaTerceiroResumo.h"
#include <QVariant>

NotaTerceiroResumo NotaTerceiroResumo::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    NotaTerceiroResumo resumo;

    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        resumo.setIdDocumentoFiscal(record.value(prefix + "id").toInt());
    else if (record.contains(prefix + "id_documento_fiscal") && !record.isNull(prefix + "id_documento_fiscal"))
        resumo.setIdDocumentoFiscal(record.value(prefix + "id_documento_fiscal").toInt());

    if (record.contains(prefix + "chave_acesso") && !record.isNull(prefix + "chave_acesso"))
        resumo.setChaveAcesso(record.value(prefix + "chave_acesso").toString());

    if (record.contains(prefix + "modelo") && !record.isNull(prefix + "modelo"))
        resumo.setModelo(record.value(prefix + "modelo").toString());

    if (record.contains(prefix + "serie") && !record.isNull(prefix + "serie"))
        resumo.setSerie(record.value(prefix + "serie").toString());

    if (record.contains(prefix + "numero") && !record.isNull(prefix + "numero"))
        resumo.setNumero(record.value(prefix + "numero").toInt());

    if (record.contains(prefix + "data_emissao") && !record.isNull(prefix + "data_emissao"))
        resumo.setDataEmissao(record.value(prefix + "data_emissao").toDateTime());
    else if (record.contains(prefix + "dh_emissao") && !record.isNull(prefix + "dh_emissao"))
        resumo.setDataEmissao(record.value(prefix + "dh_emissao").toDateTime());

    if (record.contains(prefix + "id_pessoa") && !record.isNull(prefix + "id_pessoa"))
        resumo.setIdFornecedor(record.value(prefix + "id_pessoa").toInt());
    else if (record.contains(prefix + "id_fornecedor") && !record.isNull(prefix + "id_fornecedor"))
        resumo.setIdFornecedor(record.value(prefix + "id_fornecedor").toInt());

    if (record.contains(prefix + "fornecedor_nome") && !record.isNull(prefix + "fornecedor_nome"))
        resumo.setFornecedorNome(record.value(prefix + "fornecedor_nome").toString());
    else if (record.contains(prefix + "razao_social") && !record.isNull(prefix + "razao_social"))
        resumo.setFornecedorNome(record.value(prefix + "razao_social").toString());

    if (record.contains(prefix + "fornecedor_cnpj") && !record.isNull(prefix + "fornecedor_cnpj"))
        resumo.setFornecedorCnpj(record.value(prefix + "fornecedor_cnpj").toString());
    else if (record.contains(prefix + "cnpj_cpf") && !record.isNull(prefix + "cnpj_cpf"))
        resumo.setFornecedorCnpj(record.value(prefix + "cnpj_cpf").toString());

    if (record.contains(prefix + "total_itens") && !record.isNull(prefix + "total_itens"))
        resumo.setTotalItens(record.value(prefix + "total_itens").toInt());

    if (record.contains(prefix + "itens_pendentes") && !record.isNull(prefix + "itens_pendentes"))
        resumo.setItensPendentes(record.value(prefix + "itens_pendentes").toInt());

    return resumo;
}

