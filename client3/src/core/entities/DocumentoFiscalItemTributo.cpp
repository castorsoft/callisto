#include "DocumentoFiscalItemTributo.h"
#include <QJsonValue>

DocumentoFiscalItemTributo::DocumentoFiscalItemTributo(int idItem, QString tipoTributo, QJsonObject dados)
    : m_idItem(idItem), m_tipoTributo(tipoTributo), m_dados(dados) { }

DocumentoFiscalItemTributo::DocumentoFiscalItemTributo(int id, int idItem, QString tipoTributo, QJsonObject dados, QDateTime criadoEm, QDateTime atualizadoEm)
    : m_id(id), m_idItem(idItem), m_tipoTributo(tipoTributo), m_dados(dados), m_criadoEm(criadoEm), m_atualizadoEm(atualizadoEm) { }

DocumentoFiscalItemTributo DocumentoFiscalItemTributo::loadFromDatabase(int id, int idItem, QString tipoTributo, QJsonObject dados, QDateTime criadoEm, QDateTime atualizadoEm)
{
    return DocumentoFiscalItemTributo(id, idItem, tipoTributo, dados, criadoEm, atualizadoEm);
}

void DocumentoFiscalItemTributo::hidratar(int id, QDateTime criadoEm, QDateTime atualizadoEm)
{
    m_id = id;
    m_criadoEm = criadoEm;
    m_atualizadoEm = atualizadoEm;
}