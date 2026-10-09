#include "DocumentoFiscalArquivo.h"

DocumentoFiscalArquivo::DocumentoFiscalArquivo(int idDocumento, const QString &conteudoXml, const QString &tipoArquivo)
    : m_idDocumento(idDocumento), m_conteudoXml(conteudoXml), m_tipoArquivo(tipoArquivo)
{
}

DocumentoFiscalArquivo::DocumentoFiscalArquivo(int id, const QUuid &uuid, int idDocumento,
                                              const QString &conteudoXml, const QString &tipoArquivo,
                                              const QDateTime &criadoEm, const QDateTime &atualizadoEm)
    : m_id(id), m_uuid(uuid), m_idDocumento(idDocumento),
      m_conteudoXml(conteudoXml), m_tipoArquivo(tipoArquivo),
      m_criadoEm(criadoEm), m_atualizadoEm(atualizadoEm)
{
}

DocumentoFiscalArquivo DocumentoFiscalArquivo::loadFromDatabase(int id, const QUuid &uuid, int idDocumento,
                                                              const QString &conteudoXml, const QString &tipoArquivo,
                                                              const QDateTime &criadoEm, const QDateTime &atualizadoEm)
{
    return DocumentoFiscalArquivo(id, uuid, idDocumento, conteudoXml, tipoArquivo, criadoEm, atualizadoEm);
}

void DocumentoFiscalArquivo::hidratar(int id, const QUuid &uuid, const QDateTime &criadoEm, const QDateTime &atualizadoEm)
{
    m_id = id;
    m_uuid = uuid;
    m_criadoEm = criadoEm;
    m_atualizadoEm = atualizadoEm;
}
