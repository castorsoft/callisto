#include "Endereco.h"

Endereco Endereco::loadFromDatabase(int internalId, QUuid uuid, QString logradouro,
                                    std::optional<QString> numero, std::optional<QString> complemento,
                                    QString bairro, QString cep, int idCidade,
                                    QDateTime criadoEm, QDateTime atualizadoEm)
{
    return Endereco(internalId, uuid, logradouro, numero, complemento, bairro, cep, idCidade, criadoEm, atualizadoEm);
}

void Endereco::hidratar(int internalId, QUuid uuid, QDateTime criadoEm, QDateTime atualizadoEm)
{
    m_id = internalId;
    m_uuid = uuid;
    m_criadoEm = criadoEm;
    m_atualizadoEm = atualizadoEm;
}

void Endereco::hidratar(QDateTime atualizadoEm)
{
    m_atualizadoEm = atualizadoEm;
}
