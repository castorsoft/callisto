#pragma once

#include <QString>

class Cidade {
public:
    Cidade() = default;
    Cidade(int id, const QString &nome, int idIbge, int idEstado)
        : m_id(id), m_nome(nome), m_idIbge(idIbge), m_idEstado(idEstado) {}

    int id() const { return m_id; }
    QString nome() const { return m_nome; }
    int idIbge() const { return m_idIbge; }
    int idEstado() const { return m_idEstado; }
    int idUF() const { return m_idEstado; }

private:
    int m_id{0};
    QString m_nome;
    int m_idIbge{0};
    int m_idEstado{0};
};
