#pragma once

#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QMetaType>
#include <QSqlRecord>
#include <optional>

class UnidadeMedida {
public:
    UnidadeMedida() = default;

    UnidadeMedida(const QString &sigla,
            const QString &descricao);

    static UnidadeMedida fromRecord(const QSqlRecord &record, const QString &prefix = "");


    std::optional<int> id() const { return m_id; }
    void setId(int id) { m_id = id; }

    std::optional<QUuid> uuid() const { return m_uuid; }
    void setUuid(const QUuid &uuid) { m_uuid = uuid; }

    void setSigla(const QString &sigla) { m_sigla = sigla; }
    QString sigla() const { return m_sigla; }

    QString descricao() const { return m_descricao; }
    void setDescricao(const QString &descricao) { m_descricao = descricao; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    void setCriadoEm(const QDateTime &criadoEm) { m_criadoEm = criadoEm; }

    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }
    void setAtualizadoEm(const QDateTime &atualizadoEm) { m_atualizadoEm = atualizadoEm; }

    QString textoExibicao() const { return QString("%1 - %2").arg(sigla(), descricao()); }

private:
    std::optional<int> m_id;
    std::optional<QUuid> m_uuid;
    QString m_sigla;
    QString m_descricao;
    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(UnidadeMedida)
