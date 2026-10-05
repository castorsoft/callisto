#include "Empresa.h"
#include <QVariant>

Empresa Empresa::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    Empresa e;
    
    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        e.setId(record.value(prefix + "id").toInt());
        
    if (record.contains(prefix + "uuid") && !record.isNull(prefix + "uuid"))
        e.setUuid(record.value(prefix + "uuid").toUuid());
        
    if (record.contains(prefix + "cnpj_cpf"))
        e.setCnpjCpf(record.value(prefix + "cnpj_cpf").toString());
        
    if (record.contains(prefix + "razao_social"))
        e.setRazaoSocial(record.value(prefix + "razao_social").toString());
        
    if (record.contains(prefix + "nome_fantasia"))
        e.setNomeFantasia(record.isNull(prefix + "nome_fantasia") ? std::nullopt : std::make_optional(record.value(prefix + "nome_fantasia").toString()));
        
    if (record.contains(prefix + "regime_tributario"))
        e.setRegimeTributario(record.value(prefix + "regime_tributario").toString());

    if (record.contains(prefix + "inscricao_estadual"))
        e.setInscricaoEstadual(record.isNull(prefix + "inscricao_estadual") ? std::nullopt : std::make_optional(record.value(prefix + "inscricao_estadual").toString()));

    if (record.contains(prefix + "inscricao_municipal"))
        e.setInscricaoMunicipal(record.isNull(prefix + "inscricao_municipal") ? std::nullopt : std::make_optional(record.value(prefix + "inscricao_municipal").toString()));
        
    if (record.contains(prefix + "id_endereco")) {
        int idEnd = record.value(prefix + "id_endereco").toInt();
        e.m_idEndereco = idEnd;
    }
        
    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        e.setCriadoEm(record.value(prefix + "criado_em").toDateTime());
        
    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        e.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return e;
}
