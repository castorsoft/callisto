#pragma once

#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QDate>
#include <QMetaType>
#include <QSqlRecord>
#include <optional>

class NCM {
public:
    NCM() = default;

    NCM(const QString &codigo,
        const QString &descricao,
        double aliquotaMunicipal,
        double aliquotaEstadual,
        double aliquotaFederal,
        const QDate &dataVigenciaInicio,
        const QDate &dataVigenciaFim);

    static NCM fromRecord(const QSqlRecord &record, const QString &prefix = "");

    std::optional<int> id() const { return m_id; }
    void setId(int id) { m_id = id; }

    std::optional<QUuid> uuid() const { return m_uuid; }
    void setUuid(const QUuid &uuid) { m_uuid = uuid; }

    void setCodigo(const QString &codigo) { m_codigo = codigo; }
    QString codigo() const { return m_codigo; }

    QString descricao() const { return m_descricao; }
    void setDescricao(const QString &descricao) { m_descricao = descricao; }

    double aliquotaMunicipal() const { return m_aliquotaMunicipal; }
    void setAliquotaMunicipal(const double &aliquotaMunicipal) { m_aliquotaMunicipal = aliquotaMunicipal; }

    double aliquotaEstadual() const { return m_aliquotaEstadual; }
    void setAliquotaEstadual(const double &aliquotaEstadual) { m_aliquotaEstadual = aliquotaEstadual; }

    double aliquotaFederal() const { return m_aliquotaFederal; }
    void setAliquotaFederal(const double &aliquotaFederal) { m_aliquotaFederal = aliquotaFederal; }

    QDate dataVigenciaInicio() const { return m_dataVigenciaInicio; }
    void setDataVigenciaInicio(const QDate &dataVigenciaInicio) { m_dataVigenciaInicio = dataVigenciaInicio; }

    QDate dataVigenciaFim() const { return m_dataVigenciaFim; }
    void setDataVigenciaFim(const QDate &dataVigenciaFim) { m_dataVigenciaFim = dataVigenciaFim; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    void setCriadoEm(const QDateTime &criadoEm) { m_criadoEm = criadoEm; }

    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }
    void setAtualizadoEm(const QDateTime &atualizadoEm) { m_atualizadoEm = atualizadoEm; }

    QString textoExibicao() const { return QString("%1 - %2").arg(codigo(), descricao()); }

private:
    std::optional<int> m_id;
    std::optional<QUuid> m_uuid;
    QString m_codigo;
    QString m_descricao;
    double m_aliquotaMunicipal;
    double m_aliquotaEstadual;
    double m_aliquotaFederal;
    QDate m_dataVigenciaInicio;
    QDate m_dataVigenciaFim;
    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(NCM)
