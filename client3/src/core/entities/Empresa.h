#pragma once

#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QMetaType>
#include <QList>
#include <optional>

#include "Endereco.h"

class Empresa {
public:
    Empresa();

    // Constructor for creating new instances before DB insert
    Empresa(QString cnpjCpf, QString razaoSocial, std::optional<QString> nomeFantasia,
            QString regimeTributario, Endereco endereco,
            std::optional<QString> inscricaoEstadual, std::optional<QString> inscricaoMunicipal,
            QList<int> contatosIds = {})
        : m_cnpjCpf(cnpjCpf), m_razaoSocial(razaoSocial), m_nomeFantasia(nomeFantasia),
          m_regimeTributario(regimeTributario), m_endereco(endereco),
          m_inscricaoEstadual(inscricaoEstadual), m_inscricaoMunicipal(inscricaoMunicipal),
          m_contatosIds(contatosIds) {}

    static Empresa loadFromDatabase(int internalId, QUuid uuid, QString cnpjCpf, QString razaoSocial,
                                    std::optional<QString> nomeFantasia, QString regimeTributario, Endereco endereco,
                                    std::optional<QString> inscricaoEstadual, std::optional<QString> inscricaoMunicipal,
                                    QList<int> contatosIds,
                                    QDateTime criadoEm, QDateTime atualizadoEm);

    std::optional<int> id() const { return m_id; }
    std::optional<QUuid> uuid() const { return m_uuid; }

    QString cnpjCpf() const { return m_cnpjCpf; }
    QString razaoSocial() const { return m_razaoSocial; }
    std::optional<QString> nomeFantasia() const { return m_nomeFantasia; }
    QString regimeTributario() const { return m_regimeTributario; }

    Endereco endereco() const { return m_endereco.value(); }
    void setEndereco(const Endereco &endereco) { m_endereco = endereco; }

    std::optional<QString> inscricaoEstadual() const { return m_inscricaoEstadual; }
    std::optional<QString> inscricaoMunicipal() const { return m_inscricaoMunicipal; }

    QList<int> contatosIds() const { return m_contatosIds; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }

    void hidratar(int internalId, QUuid uuid, QDateTime criadoEm, QDateTime atualizadoEm);
    void hidratar(QDateTime atualizadoEm);

private:
    Empresa(int internalId, QUuid uuid, QString cnpjCpf, QString razaoSocial,
            std::optional<QString> nomeFantasia, QString regimeTributario, Endereco endereco,
            std::optional<QString> inscricaoEstadual, std::optional<QString> inscricaoMunicipal,
            QList<int> contatosIds, QDateTime criadoEm, QDateTime atualizadoEm)
        : m_id(internalId), m_uuid(uuid), m_cnpjCpf(cnpjCpf), m_razaoSocial(razaoSocial),
          m_nomeFantasia(nomeFantasia), m_regimeTributario(regimeTributario), m_endereco(endereco),
          m_inscricaoEstadual(inscricaoEstadual), m_inscricaoMunicipal(inscricaoMunicipal),
          m_contatosIds(contatosIds), m_criadoEm(criadoEm), m_atualizadoEm(atualizadoEm) {}

    std::optional<int> m_id;
    std::optional<QUuid> m_uuid;

    QString m_cnpjCpf;
    QString m_razaoSocial;
    std::optional<QString> m_nomeFantasia;
    QString m_regimeTributario;
    std::optional<Endereco> m_endereco;
    std::optional<QString> m_inscricaoEstadual;
    std::optional<QString> m_inscricaoMunicipal;

    QList<int> m_contatosIds;

    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(Empresa)
