#pragma once

#include <QString>
#include <QUuid>
#include <QDateTime>

class Arquivo {
public:
    Arquivo() = default;
    Arquivo(QString caminhoArquivo, QString nomeArquivo, bool processado): m_caminhoArquivo(caminhoArquivo), m_nomeArquivo(nomeArquivo), m_processado(processado) {};

    static Arquivo loadFromDatabase(int internalId, QUuid uuid, QString caminhoArquivo, QString nomeArquivo, bool processado, bool possuiErro, std::optional<QString> mensagemErro, std::optional<bool> processarNovamente, QDateTime criadoEm, QDateTime atualizadoEm);

    std::optional<int> id() const { return m_id; }
    std::optional<QUuid> uuid() const { return m_uuid; }
    QString caminhoArquivo() const { return m_caminhoArquivo; }
    QString nomeArquivo() const { return m_nomeArquivo; }
    bool processado() const { return m_processado; }
    bool possuiErro() const { return m_possuiErro; }
    std::optional<QString> mensagemErro() const { return m_mensagemErro; }
    std::optional<bool> processarNovamente() const { return m_processarNovamente; }

    std::optional<QDateTime> criadoEm() { return m_criadoEm; }
    std::optional<QDateTime> atualizadoEm() { return m_atualizadoEm; }

    void definirArquivo(QString caminhoArquivo);
    void definirComoProcessado();
    void definirComErro(QString mensagemErro, bool tentarNovamente);

    void hidratar(int internalId, QUuid uuid, QDateTime criadoEm, QDateTime atualizadoEm);
    void hidratar(QDateTime atualizadoEm);
private:
    Arquivo(int internalId, QUuid uuid, QString caminhoArquivo, QString nomeArquivo, bool processado, bool possuiErro, std::optional<QString> mensagemErro, std::optional<bool> processarNovamente, QDateTime criadoEm, QDateTime atualizadoEm): m_id(internalId), m_uuid(uuid), m_caminhoArquivo(caminhoArquivo), m_nomeArquivo(nomeArquivo), m_processado(processado), m_possuiErro(possuiErro), m_mensagemErro(mensagemErro), m_processarNovamente(processarNovamente), m_criadoEm(criadoEm), m_atualizadoEm(atualizadoEm) {};

    int m_id = -1;
    std::optional<QUuid> m_uuid;
    QString m_caminhoArquivo;
    QString m_nomeArquivo;
    bool m_processado;

    bool m_possuiErro;
    std::optional<QString> m_mensagemErro;
    std::optional<bool> m_processarNovamente;

    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};