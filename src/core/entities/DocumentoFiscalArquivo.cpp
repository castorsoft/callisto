#include "DocumentoFiscalArquivo.h"

DocumentoFiscalArquivo::DocumentoFiscalArquivo(int idDocumento, const QString &conteudoXml, const QString &tipoArquivo)
    : m_idDocumento(idDocumento), m_conteudoXml(conteudoXml), m_tipoArquivo(tipoArquivo)
{
}

DocumentoFiscalArquivo DocumentoFiscalArquivo::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    DocumentoFiscalArquivo dfa;

    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        dfa.setId(record.value(prefix + "id").toInt());

    if (record.contains(prefix + "uuid") && !record.isNull(prefix + "uuid"))
        dfa.setUuid(record.value(prefix + "uuid").toUuid());

    if (record.contains(prefix + "id_documento") && !record.isNull(prefix + "id_documento"))
        dfa.m_idDocumento = record.value(prefix + "id_documento").toInt();

    if (record.contains(prefix + "conteudo_xml") && !record.isNull(prefix + "conteudo_xml"))
        dfa.m_conteudoXml = record.value(prefix + "conteudo_xml").toString();

    if (record.contains(prefix + "tipo_arquivo") && !record.isNull(prefix + "tipo_arquivo"))
        dfa.m_tipoArquivo = record.value(prefix + "tipo_arquivo").toString();

    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        dfa.setCriadoEm(record.value(prefix + "criado_em").toDateTime());

    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        dfa.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return dfa;
}