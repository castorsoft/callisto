#pragma once

#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QMetaType>
#include <QSqlRecord>
#include <optional>

class Arquivo {
public:
    Arquivo() = default;

    Arquivo(QString caminhoArquivo, QString nomeArquivo, bool processado) {
        m_caminhoArquivo = caminhoArquivo;
        m_nomeArquivo = nomeArquivo;
        m_processado = processado;
    }

    static Arquivo fromRecord(const QSqlRecord &record, const QString &prefix = "");

    std::optional<int> id() const { return m_id; }
    void setId(int id) { m_id = id; }

    std::optional<QUuid> uuid() const { return m_uuid; }
    void setUuid(const QUuid &uuid) { m_uuid = uuid; }

    QString caminhoArquivo() const { return m_caminhoArquivo; }
    QString nomeArquivo() const { return m_nomeArquivo; }

    bool processado() const { return m_processado; }
    bool possuiErro() const { return m_possuiErro; }
    std::optional<QString> mensagemErro() const { return m_mensagemErro; }
    std::optional<bool> processarNovamente() const { return m_processarNovamente; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    void setCriadoEm(const QDateTime &criadoEm) { m_criadoEm = criadoEm; }

    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }
    void setAtualizadoEm(const QDateTime &atualizadoEm) { m_atualizadoEm = atualizadoEm; }

    void definirArquivo(QString caminhoArquivo);
    void definirComoProcessado();
    void definirComErro(QString mensagemErro, bool processarNovamente);

private:
    std::optional<int> m_id;
    std::optional<QUuid> m_uuid;

    QString m_caminhoArquivo;
    QString m_nomeArquivo;

    bool m_processado = false;
    bool m_possuiErro = false;
    std::optional<QString> m_mensagemErro;
    std::optional<bool> m_processarNovamente;

    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(Arquivo)
