#include "DocumentoFiscalItemTributo.h"
#include <QJsonValue>

DocumentoFiscalItemTributo::DocumentoFiscalItemTributo(int idItem, QString tipoTributo, QJsonObject dados)
    : m_idItem(idItem), m_tipoTributo(tipoTributo), m_dados(dados) { }

DocumentoFiscalItemTributo DocumentoFiscalItemTributo::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    DocumentoFiscalItemTributo dfit;

    if (record.contains(prefix + "id"))
        dfit.setId(record.value(prefix + "id").toInt());

    if (record.contains(prefix + "id_item"))
        dfit.m_idItem = record.value(prefix + "id_item").toInt();

    if (record.contains(prefix + "tipo_tributo"))
        dfit.m_tipoTributo = record.value(prefix + "tipo_tributo").toString();

    if (record.contains(prefix + "dados"))
        dfit.m_dados = record.value(prefix + "dados").toJsonObject();

    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        dfit.setCriadoEm(record.value(prefix + "criado_em").toDateTime());

    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        dfit.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return dfit;
}