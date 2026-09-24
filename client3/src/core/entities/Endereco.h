#pragma once

#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QMetaType>
#include <optional>

class Endereco {
public:
    Endereco() = default;

    Endereco(QString logradouro, std::optional<QString> numero, std::optional<QString> complemento,
             QString bairro, QString cep, int idCidade)
        : m_logradouro(logradouro), m_numero(numero), m_complemento(complemento),
          m_bairro(bairro), m_cep(cep), m_idCidade(idCidade) {}

    static Endereco loadFromDatabase(int internalId, QUuid uuid, QString logradouro,
                                     std::optional<QString> numero, std::optional<QString> complemento,
                                     QString bairro, QString cep, int idCidade,
                                     QDateTime criadoEm, QDateTime atualizadoEm);

    std::optional<int> id() const { return m_id; }
    std::optional<QUuid> uuid() const { return m_uuid; }

    QString logradouro() const { return m_logradouro; }
    std::optional<QString> numero() const { return m_numero; }
    std::optional<QString> complemento() const { return m_complemento; }
    QString bairro() const { return m_bairro; }
    QString cep() const { return m_cep; }
    int idCidade() const { return m_idCidade; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }

    void hidratar(int internalId, QUuid uuid, QDateTime criadoEm, QDateTime atualizadoEm);
    void hidratar(QDateTime atualizadoEm);

private:
    Endereco(int internalId, QUuid uuid, QString logradouro, std::optional<QString> numero,
             std::optional<QString> complemento, QString bairro, QString cep, int idCidade,
             QDateTime criadoEm, QDateTime atualizadoEm)
        : m_id(internalId), m_uuid(uuid), m_logradouro(logradouro), m_numero(numero),
          m_complemento(complemento), m_bairro(bairro), m_cep(cep), m_idCidade(idCidade),
          m_criadoEm(criadoEm), m_atualizadoEm(atualizadoEm) {}

    std::optional<int> m_id;
    std::optional<QUuid> m_uuid;

    QString m_logradouro;
    std::optional<QString> m_numero;
    std::optional<QString> m_complemento;
    QString m_bairro;
    QString m_cep;
    int m_idCidade;

    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(Endereco)
