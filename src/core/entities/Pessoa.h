#pragma once

#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QMetaType>
#include <QSqlRecord>
#include <optional>
#include "Endereco.h"
#include "PessoaContabilista.h"

class Pessoa {
public:
    Pessoa() = default;

    Pessoa(QString cnpjCpf, QString razaoSocial, std::optional<QString> nomeFantasia,
           std::optional<QString> inscricaoEstadual, QString fornecedor, QString contabilista,
           Endereco endereco, int idEmpresa = 1)
        : m_cnpjCpf(cnpjCpf), m_razaoSocial(razaoSocial), m_nomeFantasia(nomeFantasia),
          m_inscricaoEstadual(inscricaoEstadual), m_fornecedor(fornecedor), m_contabilista(contabilista),
          m_endereco(endereco), m_idEmpresa(idEmpresa) {}

    static Pessoa fromRecord(const QSqlRecord &record, const QString &prefix = "");

    std::optional<int> id() const { return m_id; }
    void setId(int id) { m_id = id; }

    std::optional<QUuid> uuid() const { return m_uuid; }
    void setUuid(const QUuid &uuid) { m_uuid = uuid; }

    int idEmpresa() const { return m_idEmpresa; }

    QString cnpjCpf() const { return m_cnpjCpf; }
    QString razaoSocial() const { return m_razaoSocial; }
    std::optional<QString> nomeFantasia() const { return m_nomeFantasia; }
    std::optional<QString> inscricaoEstadual() const { return m_inscricaoEstadual; }
    QString fornecedor() const { return m_fornecedor; }
    QString contabilista() const { return m_contabilista; }

    int idEndereco() const { return m_idEndereco; }
    Endereco endereco() const { return m_endereco.value(); }

    std::optional<PessoaContabilista> pessoaContabilista() const { return m_pessoaContabilista; }
    void setPessoaContabilista(const std::optional<PessoaContabilista> &val) { m_pessoaContabilista = val; }

    QString textoExibicao() const { return QString("%1 - %2").arg(cnpjCpf(), razaoSocial()); }

    void setIdEmpresa(int val) { m_idEmpresa = val; }
    void setCnpjCpf(const QString &val) { m_cnpjCpf = val; }
    void setRazaoSocial(const QString &val) { m_razaoSocial = val; }
    void setNomeFantasia(const std::optional<QString> &val) { m_nomeFantasia = val; }
    void setInscricaoEstadual(const std::optional<QString> &val) { m_inscricaoEstadual = val; }
    void setFornecedor(const QString &val) { m_fornecedor = val; }
    void setContabilista(const QString &val) { m_contabilista = val; }
    void setEndereco(const Endereco &val) { m_endereco = val; if(val.id().has_value()) m_idEndereco = val.id().value(); }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    void setCriadoEm(const QDateTime &criadoEm) { m_criadoEm = criadoEm; }

    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }
    void setAtualizadoEm(const QDateTime &atualizadoEm) { m_atualizadoEm = atualizadoEm; }

private:

    std::optional<int> m_id;
    std::optional<QUuid> m_uuid;
    int m_idEmpresa = 1;

    QString m_cnpjCpf;
    QString m_razaoSocial;
    std::optional<QString> m_nomeFantasia;
    std::optional<QString> m_inscricaoEstadual;
    QString m_fornecedor = "N"; // "S" or "N"
    QString m_contabilista = "N"; // "S" or "N"

    int m_idEndereco = -1;
    std::optional<Endereco> m_endereco;
    std::optional<PessoaContabilista> m_pessoaContabilista;

    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(Pessoa)
