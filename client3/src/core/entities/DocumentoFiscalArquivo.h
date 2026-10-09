#pragma once

#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QMetaType>
#include <optional>

class DocumentoFiscalArquivo {
public:
    DocumentoFiscalArquivo() = default;
    DocumentoFiscalArquivo(int idDocumento, const QString &conteudoXml, const QString &tipoArquivo = "DOCUMENTO_FISCAL");

    static DocumentoFiscalArquivo loadFromDatabase(int id, const QUuid &uuid, int idDocumento,
                                                  const QString &conteudoXml, const QString &tipoArquivo,
                                                  const QDateTime &criadoEm, const QDateTime &atualizadoEm);

    std::optional<int> id() const { return m_id; }
    void setId(int id) { m_id = id; }

    std::optional<QUuid> uuid() const { return m_uuid; }
    void setUuid(const QUuid &uuid) { m_uuid = uuid; }

    int idDocumento() const { return m_idDocumento; }
    void setIdDocumento(int idDocumento) { m_idDocumento = idDocumento; }

    QString conteudoXml() const { return m_conteudoXml; }
    void setConteudoXml(const QString &conteudoXml) { m_conteudoXml = conteudoXml; }

    QString tipoArquivo() const { return m_tipoArquivo; }
    void setTipoArquivo(const QString &tipoArquivo) { m_tipoArquivo = tipoArquivo; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    void setCriadoEm(const QDateTime &criadoEm) { m_criadoEm = criadoEm; }

    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }
    void setAtualizadoEm(const QDateTime &atualizadoEm) { m_atualizadoEm = atualizadoEm; }

    void hidratar(int id, const QUuid &uuid, const QDateTime &criadoEm, const QDateTime &atualizadoEm);

private:
    DocumentoFiscalArquivo(int id, const QUuid &uuid, int idDocumento,
                          const QString &conteudoXml, const QString &tipoArquivo,
                          const QDateTime &criadoEm, const QDateTime &atualizadoEm);

    std::optional<int> m_id;
    std::optional<QUuid> m_uuid;
    int m_idDocumento = -1;
    QString m_conteudoXml;
    QString m_tipoArquivo = "DOCUMENTO_FISCAL";
    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(DocumentoFiscalArquivo)
