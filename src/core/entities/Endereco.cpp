#include "Endereco.h"
#include <QVariant>

Endereco Endereco::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    Endereco e;
    
    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        e.setId(record.value(prefix + "id").toInt());
        
    if (record.contains(prefix + "uuid") && !record.isNull(prefix + "uuid"))
        e.setUuid(record.value(prefix + "uuid").toUuid());
        
    if (record.contains(prefix + "logradouro"))
        e.m_logradouro = record.value(prefix + "logradouro").toString();
        
    if (record.contains(prefix + "numero"))
        e.m_numero = record.isNull(prefix + "numero") ? std::nullopt : std::make_optional(record.value(prefix + "numero").toString());
        
    if (record.contains(prefix + "complemento"))
        e.m_complemento = record.isNull(prefix + "complemento") ? std::nullopt : std::make_optional(record.value(prefix + "complemento").toString());
        
    if (record.contains(prefix + "bairro"))
        e.m_bairro = record.value(prefix + "bairro").toString();
        
    if (record.contains(prefix + "cep"))
        e.m_cep = record.value(prefix + "cep").toString();
        
    if (record.contains(prefix + "id_cidade"))
        e.m_idCidade = record.value(prefix + "id_cidade").toInt();
        
    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        e.setCriadoEm(record.value(prefix + "criado_em").toDateTime());
        
    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        e.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return e;
}
