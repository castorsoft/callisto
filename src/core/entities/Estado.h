#pragma once

#include <QString>

class Estado {
public:
    Estado() = default;
    Estado(int id, const QString &nome, const QString &sigla, int idIbge)
        : m_id(id), m_nome(nome), m_sigla(sigla), m_idIbge(idIbge) {}

    int id() const { return m_id; }
    QString nome() const { return m_nome; }
    QString sigla() const { return m_sigla; }
    int idIbge() const { return m_idIbge; }

private:
    int m_id;
    QString m_nome;
    QString m_sigla;
    int m_idIbge;
};
