#include "UnidadeMedida.h"
#include <QVariant>

UnidadeMedida::UnidadeMedida(const QString &sigla, const QString &descricao): m_sigla(sigla), m_descricao(descricao) { }

UnidadeMedida UnidadeMedida::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    UnidadeMedida um;

    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        um.setId(record.value(prefix + "id").toInt());

    if (record.contains(prefix + "uuid") && !record.isNull(prefix + "uuid"))
        um.setUuid(record.value(prefix + "uuid").toUuid());

    if (record.contains(prefix + "sigla") && !record.isNull(prefix + "sigla"))
        um.setSigla(record.value(prefix + "sigla").toString());

    if (record.contains(prefix + "descricao") && !record.isNull(prefix + "descricao"))
        um.setDescricao(record.value(prefix + "descricao").toString());

    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        um.setCriadoEm(record.value(prefix + "criado_em").toDateTime());

    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        um.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return um;
}
