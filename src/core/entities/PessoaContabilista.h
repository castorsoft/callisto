#pragma once

#include <QString>
#include <QDateTime>
#include <QMetaType>
#include <QSqlRecord>
#include <optional>

class PessoaContabilista {
public:
    PessoaContabilista() = default;

    PessoaContabilista(int idPessoa, const QString &crc, const QString &cpf = "")
        : m_idPessoa(idPessoa), m_crc(crc), m_cpf(cpf) {}

    static PessoaContabilista fromRecord(const QSqlRecord &record, const QString &prefix = "");

    std::optional<int> id() const { return m_id; }
    void setId(int id) { m_id = id; }

    int idPessoa() const { return m_idPessoa; }
    void setIdPessoa(int idPessoa) { m_idPessoa = idPessoa; }

    QString crc() const { return m_crc; }
    void setCrc(const QString &crc) { m_crc = crc; }

    QString cpf() const { return m_cpf; }
    void setCpf(const QString &cpf) { m_cpf = cpf; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    void setCriadoEm(const QDateTime &criadoEm) { m_criadoEm = criadoEm; }

    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }
    void setAtualizadoEm(const QDateTime &atualizadoEm) { m_atualizadoEm = atualizadoEm; }

private:
    std::optional<int> m_id;
    int m_idPessoa = -1;
    QString m_crc;
    QString m_cpf;

    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(PessoaContabilista)

