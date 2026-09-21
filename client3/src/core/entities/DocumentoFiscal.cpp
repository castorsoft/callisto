#include "DocumentoFiscal.h"

DocumentoFiscal::DocumentoFiscal(QString chaveAcesso, QString tipoOperacao, QString tipoEmissao,
                                 int idPessoa, QString modelo, QString serie, int numero,
                                 QDateTime dhEmissao, QJsonObject dados)
    : m_chaveAcesso(chaveAcesso), m_tipoOperacao(tipoOperacao), m_tipoEmissao(tipoEmissao),
      m_idPessoa(idPessoa), m_modelo(modelo), m_serie(serie), m_numero(numero),
      m_dhEmissao(dhEmissao), m_dados(dados)
{
}

DocumentoFiscal::DocumentoFiscal(int id, QString chaveAcesso, QString tipoOperacao, QString tipoEmissao,
                                 int idPessoa, QString modelo, QString serie, int numero,
                                 QDateTime dhEmissao, QJsonObject dados, QDateTime criadoEm,
                                 QDateTime atualizadoEm)
    : m_id(id), m_chaveAcesso(chaveAcesso), m_tipoOperacao(tipoOperacao), m_tipoEmissao(tipoEmissao),
      m_idPessoa(idPessoa), m_modelo(modelo), m_serie(serie), m_numero(numero),
      m_dhEmissao(dhEmissao), m_dados(dados), m_criadoEm(criadoEm), m_atualizadoEm(atualizadoEm)
{
}

DocumentoFiscal DocumentoFiscal::loadFromDatabase(int id, QString chaveAcesso, QString tipoOperacao,
                                                  QString tipoEmissao, int idPessoa, QString modelo,
                                                  QString serie, int numero, QDateTime dhEmissao,
                                                  QJsonObject dados, QDateTime criadoEm, QDateTime atualizadoEm)
{
    return DocumentoFiscal(id, chaveAcesso, tipoOperacao, tipoEmissao, idPessoa, modelo,
                           serie, numero, dhEmissao, dados, criadoEm, atualizadoEm);
}

void DocumentoFiscal::hidratar(int id, QDateTime criadoEm, QDateTime atualizadoEm)
{
    m_id = id;
    m_criadoEm = criadoEm;
    m_atualizadoEm = atualizadoEm;
}
