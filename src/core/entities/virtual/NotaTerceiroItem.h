#pragma once

#include <QString>
#include <QSqlRecord>
#include <QMetaType>
#include <optional>

class NotaTerceiroItem {
public:
    NotaTerceiroItem() = default;

    static NotaTerceiroItem fromRecord(const QSqlRecord &record, const QString &prefix = "");

    int idItem() const { return m_idItem; }
    void setIdItem(int id) { m_idItem = id; }

    int idDocumentoFiscal() const { return m_idDocumentoFiscal; }
    void setIdDocumentoFiscal(int id) { m_idDocumentoFiscal = id; }

    int numeroItem() const { return m_numeroItem; }
    void setNumeroItem(int numero) { m_numeroItem = numero; }

    QString codigoProduto() const { return m_codigoProduto; }
    void setCodigoProduto(const QString &codigo) { m_codigoProduto = codigo; }

    QString codigoBarras() const { return m_codigoBarras; }
    void setCodigoBarras(const QString &codigoBarras) { m_codigoBarras = codigoBarras; }

    QString descricao() const { return m_descricao; }
    void setDescricao(const QString &descricao) { m_descricao = descricao; }

    QString unidadeMedida() const { return m_unidadeMedida; }
    void setUnidadeMedida(const QString &unidade) { m_unidadeMedida = unidade; }

    double quantidade() const { return m_quantidade; }
    void setQuantidade(double quantidade) { m_quantidade = quantidade; }

    double valorUnitario() const { return m_valorUnitario; }
    void setValorUnitario(double valorUnitario) { m_valorUnitario = valorUnitario; }

    double valorTotal() const { return m_valorTotal; }
    void setValorTotal(double valorTotal) { m_valorTotal = valorTotal; }

    QString ncm() const { return m_ncm; }
    void setNcm(const QString &ncm) { m_ncm = ncm; }

    bool vinculado() const { return m_vinculado; }
    void setVinculado(bool vinculado) { m_vinculado = vinculado; }

    std::optional<int> idProdutoFornecedor() const { return m_idProdutoFornecedor; }
    void setIdProdutoFornecedor(const std::optional<int> &id) { m_idProdutoFornecedor = id; }

    std::optional<int> idProduto() const { return m_idProduto; }
    void setIdProduto(const std::optional<int> &id) { m_idProduto = id; }

    QString produtoCodigoInterno() const { return m_produtoCodigoInterno; }
    void setProdutoCodigoInterno(const QString &codigo) { m_produtoCodigoInterno = codigo; }

    QString produtoDescricao() const { return m_produtoDescricao; }
    void setProdutoDescricao(const QString &descricao) { m_produtoDescricao = descricao; }

    double multiplicador() const { return m_multiplicador; }
    void setMultiplicador(double multiplicador) { m_multiplicador = multiplicador; }

    std::optional<int> idUnidadeMedidaInterna() const { return m_idUnidadeMedidaInterna; }
    void setIdUnidadeMedidaInterna(const std::optional<int> &id) { m_idUnidadeMedidaInterna = id; }

    QString siglaUnidadeMedidaInterna() const { return m_siglaUnidadeMedidaInterna; }
    void setSiglaUnidadeMedidaInterna(const QString &sigla) { m_siglaUnidadeMedidaInterna = sigla; }

private:
    int m_idItem{0};
    int m_idDocumentoFiscal{0};
    int m_numeroItem{0};
    QString m_codigoProduto;
    QString m_codigoBarras;
    QString m_descricao;
    QString m_unidadeMedida;
    double m_quantidade{0.0};
    double m_valorUnitario{0.0};
    double m_valorTotal{0.0};
    QString m_ncm;

    bool m_vinculado{false};
    std::optional<int> m_idProdutoFornecedor;
    std::optional<int> m_idProduto;
    QString m_produtoCodigoInterno;
    QString m_produtoDescricao;
    double m_multiplicador{1.0};
    std::optional<int> m_idUnidadeMedidaInterna;
    QString m_siglaUnidadeMedidaInterna;
};

Q_DECLARE_METATYPE(NotaTerceiroItem)
