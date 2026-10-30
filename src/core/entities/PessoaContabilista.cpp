#include "PessoaContabilista.h"
#include <QVariant>

PessoaContabilista PessoaContabilista::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    PessoaContabilista pc;

    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        pc.setId(record.value(prefix + "id").toInt());

    if (record.contains(prefix + "id_pessoa") && !record.isNull(prefix + "id_pessoa"))
        pc.setIdPessoa(record.value(prefix + "id_pessoa").toInt());

    if (record.contains(prefix + "crc") && !record.isNull(prefix + "crc"))
        pc.setCrc(record.value(prefix + "crc").toString().trimmed());

    if (record.contains(prefix + "cpf") && !record.isNull(prefix + "cpf"))
        pc.setCpf(record.value(prefix + "cpf").toString().trimmed());

    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        pc.setCriadoEm(record.value(prefix + "criado_em").toDateTime());

    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        pc.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return pc;
}

