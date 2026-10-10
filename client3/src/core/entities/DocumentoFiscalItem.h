#pragma once

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QMetaType>
#include <QSqlRecord>
#include <optional>

#include "DocumentoFiscalItemTributo.h"

class DocumentoFiscalItem {
public:
    DocumentoFiscalItem() = default;

    // Construtor principal para criação antes de salvar no banco
    DocumentoFiscalItem(int idDocumentoFiscal, int numeroItem, QString codigoProduto, QString descricao,
                        QString unidadeMedida, double quantidade,
                        double valorUnitario, double valorTotal, double valorDesconto, QJsonObject dados);

    static DocumentoFiscalItem fromRecord(const QSqlRecord &record, const QString &prefix = "");

    std::optional<int> id() const { return m_id; }
    void setId(int id) { m_id = id; }

    int idDocumentoFiscal() const { return m_idDocumentoFiscal; }
    int numeroItem() const { return m_numeroItem; }
    QString codigoProduto() const { return m_codigoProduto; }
    QString descricao() const { return m_descricao; }
    QString unidadeMedida() const { return m_unidadeMedida; }
    double quantidade() const { return m_quantidade; }
    double valorUnitario() const { return m_valorUnitario; }
    double valorTotal() const { return m_valorTotal; }
    double valorDesconto() const { return m_valorDesconto; }
    QJsonObject dados() const { return m_dados; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    void setCriadoEm(const QDateTime &criadoEm) { m_criadoEm = criadoEm; }

    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }
    void setAtualizadoEm(const QDateTime &atualizadoEm) { m_atualizadoEm = atualizadoEm; }

    // Getters para impostos e dados específicos dentro de "dados" JSON
    QString cstIcms() const;
    QString cstPis() const;
    QString cstCofins() const;
    QString cstIpi() const;

    double baseIcms() const;
    double valorIcms() const;
    double aliquotaIcms() const;

    const std::vector<DocumentoFiscalItemTributo>& tributos() const { return m_tributos; }
    void adicionarTributo(const DocumentoFiscalItemTributo& tributo) { m_tributos.push_back(tributo); }

private:
    int m_id = -1;
    int m_idDocumentoFiscal = -1;
    int m_numeroItem = 0;

    QString m_codigoProduto;
    QString m_descricao;
    QString m_unidadeMedida;

    double m_quantidade = 0.0;
    double m_valorUnitario = 0.0;
    double m_valorTotal = 0.0;
    double m_valorDesconto = 0.0;

    QJsonObject m_dados;

    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;

    std::vector<DocumentoFiscalItemTributo> m_tributos;
};

Q_DECLARE_METATYPE(DocumentoFiscalItem)
