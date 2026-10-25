#pragma once

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QMetaType>
#include <QSqlRecord>
#include <optional>

class DocumentoFiscalItemTributo {
public:
    DocumentoFiscalItemTributo() = default;

    DocumentoFiscalItemTributo(int idItem, QString tipoTributo, QJsonObject dados);
    static DocumentoFiscalItemTributo fromRecord(const QSqlRecord &record, const QString &prefix = "");

    std::optional<int> id() const { return m_id; }
    void setId(int id) { m_id = id; }

    int idItem() const { return m_idItem; }
    QString tipoTributo() const { return m_tipoTributo; }
    QJsonObject dados() const { return m_dados; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    void setCriadoEm(const QDateTime& criadoEm) { m_criadoEm = criadoEm; }

    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }
    void setAtualizadoEm(const QDateTime& atualizadoEm) { m_atualizadoEm = atualizadoEm; }

private:
    std::optional<int> m_id;
    int m_idItem = -1;

    QString m_tipoTributo;

    QJsonObject m_dados;

    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(DocumentoFiscalItemTributo)
