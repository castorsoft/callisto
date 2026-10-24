#pragma once

#include <QString>
#include <QUuid>
#include <QDateTime>
#include <QMetaType>
#include <QSqlRecord>
#include <optional>

namespace TipoProdutoAtividade {
    inline const QString MercadoriaRevenda    = "00"; // 00 – Mercadoria para Revenda
    inline const QString MateriaPrima         = "01"; // 01 – Matéria-Prima
    inline const QString Embalagem            = "02"; // 02 – Embalagem
    inline const QString ProdutoEmProcesso    = "03"; // 03 – Produto em Processo
    inline const QString ProdutoAcabado       = "04"; // 04 – Produto Acabado
    inline const QString Subproduto           = "05"; // 05 – Subproduto
    inline const QString ProdutoIntermediario = "06"; // 06 – Produto Intermediário
    inline const QString MaterialUsoConsumo   = "07"; // 07 – Material de Uso e Consumo
    inline const QString AtivoImobilizado     = "08"; // 08 – Ativo Imobilizado
    inline const QString Servicos             = "09"; // 09 – Serviços
    inline const QString OutrosInsumos        = "10"; // 10 – Outros Insumos
    inline const QString Outras               = "99"; // 99 – Outras
}

class Produto {
public:
    Produto() = default;

    Produto(const QString &descricao,
            int idUnidadeMedida,
            int idNCM,
            const QString &tipoAtividade = TipoProdutoAtividade::MercadoriaRevenda,
            const std::optional<QString> &codigoBarra = std::nullopt,
            const std::optional<QString> &codigoInterno = std::nullopt,
            const std::optional<int> &idAnp = std::nullopt);

    static Produto fromRecord(const QSqlRecord &record, const QString &prefix = "");

    std::optional<int> id() const { return m_id; }
    void setId(int id) { m_id = id; }

    std::optional<QUuid> uuid() const { return m_uuid; }
    void setUuid(const QUuid &uuid) { m_uuid = uuid; }

    QString descricao() const { return m_descricao; }
    void setDescricao(const QString &descricao) { m_descricao = descricao; }

    std::optional<QString> codigoBarra() const { return m_codigoBarra; }
    void setCodigoBarra(const std::optional<QString> &codigoBarra) { m_codigoBarra = codigoBarra; }

    int idUnidadeMedida() const { return m_idUnidadeMedida; }
    void setIdUnidadeMedida(int idUnidadeMedida) { m_idUnidadeMedida = idUnidadeMedida; }

    QString tipoAtividade() const { return m_tipoAtividade; }
    void setTipoAtividade(const QString &tipoAtividade) { m_tipoAtividade = tipoAtividade; }

    int idNCM() const { return m_idNcm; }
    void setIdNcm(int idNCM) { m_idNcm = idNCM; }

    std::optional<QString> codigoInterno() const { return m_codigoInterno; }
    void setCodigoInterno(const std::optional<QString> &codigoInterno) { m_codigoInterno = codigoInterno; }

    std::optional<int> idAnp() const { return m_idAnp; }
    void setIdAnp(const std::optional<int> &idAnp) { m_idAnp = idAnp; }

    std::optional<QDateTime> criadoEm() const { return m_criadoEm; }
    void setCriadoEm(const QDateTime &criadoEm) { m_criadoEm = criadoEm; }

    std::optional<QDateTime> atualizadoEm() const { return m_atualizadoEm; }
    void setAtualizadoEm(const QDateTime &atualizadoEm) { m_atualizadoEm = atualizadoEm; }

private:
    std::optional<int> m_id;
    std::optional<QUuid> m_uuid;
    QString m_descricao;
    std::optional<QString> m_codigoBarra;
    int m_idUnidadeMedida;

    QString m_tipoAtividade{TipoProdutoAtividade::MercadoriaRevenda};
    int m_idNcm;
    std::optional<QString> m_codigoInterno;
    std::optional<int> m_idAnp;
    std::optional<QDateTime> m_criadoEm;
    std::optional<QDateTime> m_atualizadoEm;
};

Q_DECLARE_METATYPE(Produto)
