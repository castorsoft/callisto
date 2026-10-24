#pragma once

#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QMetaType>
#include <QSqlRecord>
#include <optional>

class ProdutoFornecedor {
public:
    ProdutoFornecedor() = default;

    ProdutoFornecedor(int idProduto,
                      int idFornecedor,
                      const QString &codigoFornecedor,
                      int idUnidadeMedida,
                      double multiplicador = 1.0,
                      const std::optional<QString> &codigoBarraCaixa = std::nullopt);

    static ProdutoFornecedor fromRecord(const QSqlRecord &record, const QString &prefix = "");

    std::optional<int> id() const { return m_id; }
    void setId(int id) { m_id = id; }

    std::optional<QUuid> uuid() const { return m_uuid; }
    void setUuid(const QUuid &uuid) { m_uuid = uuid; }

    int idProduto() const { return m_idProduto; }
    void setIdProduto(int idProduto) { m_idProduto = idProduto; }

    int idFornecedor() const { return m_idFornecedor; }
    void setIdFornecedor(int idFornecedor) { m_idFornecedor = idFornecedor; }

    QString codigoFornecedor() const { return m_codigoFornecedor; }
    void setCodigoFornecedor(const QString &codigoFornecedor) { m_codigoFornecedor = codigoFornecedor; }

    int idUnidadeMedida() const { return m_idUnidadeMedida; }
    void setIdUnidadeMedida(int idUnidadeMedida) { m_idUnidadeMedida = idUnidadeMedida; }

    double multiplicador() const { return m_multiplicador; }
    void setMultiplicador(double multiplicador) { m_multiplicador = multiplicador; }

    std::optional<QString> codigoBarraCaixa() const { return m_codigoBarraCaixa; }
    void setCodigoBarraCaixa(const std::optional<QString> &codigoBarraCaixa) { m_codigoBarraCaixa = codigoBarraCaixa; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    void setCriadoEm(const QDateTime &criadoEm) { m_criadoEm = criadoEm; }

    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }
    void setAtualizadoEm(const QDateTime &atualizadoEm) { m_atualizadoEm = atualizadoEm; }

private:
    std::optional<int> m_id;
    std::optional<QUuid> m_uuid;
    int m_idProduto{0};
    int m_idFornecedor{0};
    QString m_codigoFornecedor;
    int m_idUnidadeMedida{0};
    double m_multiplicador{1.0};
    std::optional<QString> m_codigoBarraCaixa;
    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(ProdutoFornecedor)

