#include "Pessoa.h"
#include <QVariant>

Pessoa Pessoa::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    Pessoa p;
    
    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        p.setId(record.value(prefix + "id").toInt());
        
    if (record.contains(prefix + "uuid") && !record.isNull(prefix + "uuid"))
        p.setUuid(record.value(prefix + "uuid").toUuid());
        
    if (record.contains(prefix + "id_empresa"))
        p.setIdEmpresa(record.value(prefix + "id_empresa").toInt());
        
    if (record.contains(prefix + "cnpj_cpf"))
        p.setCnpjCpf(record.value(prefix + "cnpj_cpf").toString());
        
    if (record.contains(prefix + "razao_social"))
        p.setRazaoSocial(record.value(prefix + "razao_social").toString());
        
    if (record.contains(prefix + "nome_fantasia"))
        p.setNomeFantasia(record.isNull(prefix + "nome_fantasia") ? std::nullopt : std::make_optional(record.value(prefix + "nome_fantasia").toString()));
        
    if (record.contains(prefix + "inscricao_estadual"))
        p.setInscricaoEstadual(record.isNull(prefix + "inscricao_estadual") ? std::nullopt : std::make_optional(record.value(prefix + "inscricao_estadual").toString()));
        
    if (record.contains(prefix + "fornecedor"))
        p.setFornecedor(record.value(prefix + "fornecedor").toString());
        
    if (record.contains(prefix + "contabilista"))
        p.setContabilista(record.value(prefix + "contabilista").toString());
        
    if (record.contains(prefix + "id_endereco")) {
        // Just setting the foreign key internally
        int idEnd = record.value(prefix + "id_endereco").toInt();
        // we can't easily populate the full Endereco here if columns aren't joined
        // but the repository will handle it or we can try parsing the Endereco from the same record
        Endereco end = Endereco::fromRecord(record, "end_");
        if (end.id().has_value()) {
            p.setEndereco(end);
        } else {
            // just set ID
            Endereco temp;
            temp.setId(idEnd);
            p.setEndereco(temp);
        }
    }
        
    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        p.setCriadoEm(record.value(prefix + "criado_em").toDateTime());
        
    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        p.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return p;
}
