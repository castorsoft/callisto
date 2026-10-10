#pragma once

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QMetaType>
#include <optional>
#include <vector>
#include <QSqlRecord>
#include "DocumentoFiscalItem.h"

namespace TipoOperacao {
    inline const QString Saida      = "saida";
    inline const QString Entrada    = "entrada";
}

namespace TipoEmissao {
    inline const QString Propria    = "propria";
    inline const QString Terceiro   = "terceiro";
}

class DocumentoFiscal {
public:
    DocumentoFiscal() = default;

    DocumentoFiscal(QString chaveAcesso, QString tipoOperacao, QString tipoEmissao, int idPessoa,
                    QString modelo, QString serie, int numero, QDateTime dhEmissao, QJsonObject dados);

    static DocumentoFiscal fromRecord(const QSqlRecord &record, const QString &prefix = "");

    std::optional<int> id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString chaveAcesso() const { return m_chaveAcesso; }
    QString tipoOperacao() const { return m_tipoOperacao; }
    QString tipoEmissao() const { return m_tipoEmissao; }
    int idPessoa() const { return m_idPessoa; }
    QString modelo() const { return m_modelo; }
    QString serie() const { return m_serie; }
    int numero() const { return m_numero; }
    QDateTime dhEmissao() const { return m_dhEmissao; }
    QJsonObject dados() const { return m_dados; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    void setCriadoEm(const QDateTime &criadoEm) { m_criadoEm = criadoEm; }

    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }
    void setAtualizadoEm(const QDateTime &atualizadoEm) { m_atualizadoEm = atualizadoEm; }

    const std::vector<DocumentoFiscalItem>& itens() const { return m_itens; }
    void adicionarItem(const DocumentoFiscalItem& item) { m_itens.push_back(item); }
    void setItens(const std::vector<DocumentoFiscalItem>& itens) { m_itens = itens; }

private:
    int m_id = -1;
    QString m_chaveAcesso;
    QString m_tipoOperacao;
    QString m_tipoEmissao;
    int m_idPessoa = -1;
    QString m_modelo;
    QString m_serie;
    int m_numero = 0;
    QDateTime m_dhEmissao;
    QJsonObject m_dados;

    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;

    std::vector<DocumentoFiscalItem> m_itens;
};

Q_DECLARE_METATYPE(DocumentoFiscal)
