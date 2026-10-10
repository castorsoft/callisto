#include "Produto.h"
#include <QVariant>

Produto::Produto(const QString &descricao,
                 int idUnidadeMedida,
                 int idNcm,
                 const QString &tipoAtividade,
                 const std::optional<QString> &codigoBarra,
                 const std::optional<QString> &codigoInterno,
                 const std::optional<int> &idAnp)
    : m_descricao(descricao),
      m_codigoBarra(codigoBarra),
      m_idUnidadeMedida(idUnidadeMedida),
      m_tipoAtividade(tipoAtividade),
      m_idNcm(idNcm),
      m_codigoInterno(codigoInterno),
      m_idAnp(idAnp) { }

Produto Produto::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    Produto p;

    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        p.setId(record.value(prefix + "id").toInt());

    if (record.contains(prefix + "uuid") && !record.isNull(prefix + "uuid"))
        p.setUuid(record.value(prefix + "uuid").toUuid());

    if (record.contains(prefix + "descricao") && !record.isNull(prefix + "descricao"))
        p.setDescricao(record.value(prefix + "descricao").toString());

    if (record.contains(prefix + "codigo_barra"))
        p.setCodigoBarra(record.isNull(prefix + "codigo_barra")
                             ? std::nullopt
                             : std::make_optional(record.value(prefix + "codigo_barra").toString()));

    if (record.contains(prefix + "id_unidade_medida") && !record.isNull(prefix + "id_unidade_medida"))
        p.setIdUnidadeMedida(record.value(prefix + "id_unidade_medida").toInt());

    if (record.contains(prefix + "tipo_atividade") && !record.isNull(prefix + "tipo_atividade"))
        p.setTipoAtividade(record.value(prefix + "tipo_atividade").toString());

    if (record.contains(prefix + "id_ncm") && !record.isNull(prefix + "id_ncm"))
        p.setIdNcm(record.value(prefix + "id_ncm").toInt());

    if (record.contains(prefix + "codigo_interno"))
        p.setCodigoInterno(record.isNull(prefix + "codigo_interno")
                               ? std::nullopt
                               : std::make_optional(record.value(prefix + "codigo_interno").toString()));

    if (record.contains(prefix + "id_anp"))
        p.setIdAnp(record.isNull(prefix + "id_anp")
                       ? std::nullopt
                       : std::make_optional(record.value(prefix + "id_anp").toInt()));

    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        p.setCriadoEm(record.value(prefix + "criado_em").toDateTime());

    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        p.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return p;
}
