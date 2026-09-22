#include "Empresa.h"

Empresa Empresa::loadFromDatabase(int internalId, QUuid uuid, QString cnpjCpf, QString razaoSocial,
                                  std::optional<QString> nomeFantasia, QString regimeTributario, int idEndereco,
                                  std::optional<QString> inscricaoEstadual, std::optional<QString> inscricaoMunicipal,
                                  QList<int> contatosIds,
                                  QDateTime criadoEm, QDateTime atualizadoEm)
{
    return Empresa(internalId, uuid, cnpjCpf, razaoSocial, nomeFantasia, regimeTributario, idEndereco,
                   inscricaoEstadual, inscricaoMunicipal, contatosIds, criadoEm, atualizadoEm);
}

void Empresa::hidratar(int internalId, QUuid uuid, QDateTime criadoEm, QDateTime atualizadoEm)
{
    m_id = internalId;
    m_uuid = uuid;
    m_criadoEm = criadoEm;
    m_atualizadoEm = atualizadoEm;
}

void Empresa::hidratar(QDateTime atualizadoEm)
{
    m_atualizadoEm = atualizadoEm;
}
