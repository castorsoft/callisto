#include "Pessoa.h"

Pessoa Pessoa::loadFromDatabase(int internalId, QUuid uuid, int idEmpresa, QString cnpjCpf, QString razaoSocial,
                                std::optional<QString> nomeFantasia, std::optional<QString> inscricaoEstadual,
                                QString fornecedor, QString contabilista, int idEndereco, Endereco endereco,
                                QDateTime criadoEm, QDateTime atualizadoEm) {
    return Pessoa(internalId, uuid, idEmpresa, cnpjCpf, razaoSocial, nomeFantasia, inscricaoEstadual,
                  fornecedor, contabilista, idEndereco, endereco, criadoEm, atualizadoEm);
}

void Pessoa::hidratar(int internalId, QUuid uuid, int idEndereco, QDateTime criadoEm, QDateTime atualizadoEm) {
    m_id = internalId;
    m_uuid = uuid;
    m_idEndereco = idEndereco;
    m_criadoEm = criadoEm;
    m_atualizadoEm = atualizadoEm;
}

void Pessoa::hidratar(QDateTime atualizadoEm) {
    m_atualizadoEm = atualizadoEm;
}
