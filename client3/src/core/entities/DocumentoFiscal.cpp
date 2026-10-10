#include "DocumentoFiscal.h"

DocumentoFiscal::DocumentoFiscal(QString chaveAcesso, QString tipoOperacao, QString tipoEmissao,
                                 int idPessoa, QString modelo, QString serie, int numero,
                                 QDateTime dhEmissao, QJsonObject dados)
    : m_chaveAcesso(chaveAcesso), m_tipoOperacao(tipoOperacao), m_tipoEmissao(tipoEmissao),
      m_idPessoa(idPessoa), m_modelo(modelo), m_serie(serie), m_numero(numero),
      m_dhEmissao(dhEmissao), m_dados(dados)
{
}

DocumentoFiscal DocumentoFiscal::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    DocumentoFiscal df;

    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        df.m_id = record.value(prefix + "id").toInt();

    if (record.contains(prefix + "chave_acesso") && !record.isNull(prefix + "chave_acesso"))
        df.m_chaveAcesso = record.value(prefix + "chave_acesso").toString();

    if (record.contains(prefix + "tipo_operacao") && !record.isNull(prefix + "tipo_operacao"))
        df.m_tipoOperacao = record.value(prefix + "tipo_operacao").toString();

    if (record.contains(prefix + "tipo_emissao") && !record.isNull(prefix + "tipo_emissao"))
        df.m_tipoEmissao = record.value(prefix + "tipo_emissao").toString();

    if (record.contains(prefix + "id_pessoa") && !record.isNull(prefix + "id_pessoa"))
        df.m_idPessoa = record.value(prefix + "id_pessoa").toInt();

    if (record.contains(prefix + "modelo") && !record.isNull(prefix + "modelo"))
        df.m_modelo = record.value(prefix + "modelo").toString();

    if (record.contains(prefix + "serie") && !record.isNull(prefix + "serie"))
        df.m_serie = record.value(prefix + "serie").toString();

    if (record.contains(prefix + "numero") && !record.isNull(prefix + "numero"))
        df.m_numero = record.value(prefix + "numero").toInt();

    if (record.contains(prefix + "dh_emissao") && !record.isNull(prefix + "dh_emissao"))
        df.m_dhEmissao = record.value(prefix + "dh_emissao").toDateTime();

    if (record.contains(prefix + "dados") && !record.isNull(prefix + "dados"))
        df.m_dados = record.value(prefix + "dados").toJsonObject();

    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        df.m_criadoEm = record.value(prefix + "criado_em").toDateTime();

    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        df.m_atualizadoEm = record.value(prefix + "atualizado_em").toDateTime();

    return df;
}
