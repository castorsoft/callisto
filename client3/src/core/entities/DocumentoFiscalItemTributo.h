#pragma once

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QMetaType>
#include <optional>

class DocumentoFiscalItemTributo {
public:
    DocumentoFiscalItemTributo() = default;

    // Construtor principal para criação antes de salvar no banco
    DocumentoFiscalItemTributo(int idItem, QString tipoTributo, QJsonObject dados);

    static DocumentoFiscalItemTributo loadFromDatabase(int id, int idItem, QString tipoTributo, QJsonObject dados, QDateTime criadoEm, QDateTime atualizadoEm);

    std::optional<int> id() const { return m_id; }
    int idItem() const { return m_idItem; }
    QString tipoTributo() const { return m_tipoTributo; }
    QJsonObject dados() const { return m_dados; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }

    void hidratar(int id, QDateTime criadoEm, QDateTime atualizadoEm);

private:
    DocumentoFiscalItemTributo(int id, int idItem, QString tipoTributo, QJsonObject dados, QDateTime criadoEm, QDateTime atualizadoEm);

    int m_id = -1;
    int m_idItem = -1;

    QString m_tipoTributo;

    QJsonObject m_dados;

    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(DocumentoFiscalItemTributo)
