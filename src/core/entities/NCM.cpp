#include "NCM.h"

NCM::NCM(const QString &codigo, const QString &descricao, double aliquotaMunicipal, double aliquotaEstadual, double aliquotaFederal, const QDate &dataVigenciaInicio, const QDate &dataVigenciaFim): m_codigo(codigo), m_descricao(descricao), m_aliquotaMunicipal(aliquotaMunicipal), m_aliquotaEstadual(aliquotaEstadual), m_aliquotaFederal(aliquotaFederal), m_dataVigenciaInicio(dataVigenciaInicio), m_dataVigenciaFim(dataVigenciaFim) { }

NCM NCM::fromRecord(const QSqlRecord &record, const QString &prefix)
{
    NCM n;

    if (record.contains(prefix + "id") && !record.isNull(prefix + "id"))
        n.setId(record.value(prefix + "id").toInt());

    if (record.contains(prefix + "uuid") && !record.isNull(prefix + "uuid"))
        n.setUuid(record.value(prefix + "uuid").toUuid());

    if (record.contains(prefix + "codigo") && !record.isNull(prefix + "codigo"))
        n.setCodigo(record.value(prefix + "codigo").toString());

    if (record.contains(prefix + "descricao") && !record.isNull(prefix + "descricao"))
        n.setDescricao(record.value(prefix + "descricao").toString());

    if (record.contains(prefix + "aliquota_municipal") && !record.isNull(prefix + "aliquota_municipal"))
        n.setAliquotaMunicipal(record.value(prefix + "aliquota_municipal").toDouble());

    if (record.contains(prefix + "aliquota_estadual") && !record.isNull(prefix + "aliquota_estadual"))
        n.setAliquotaEstadual(record.value(prefix + "aliquota_estadual").toDouble());

    if (record.contains(prefix + "aliquota_federal") && !record.isNull(prefix + "aliquota_federal"))
        n.setAliquotaFederal(record.value(prefix + "aliquota_federal").toDouble());

    if (record.contains(prefix + "data_vigencia_inicio") && !record.isNull(prefix + "data_vigencia_inicio"))
        n.setDataVigenciaInicio(record.value(prefix + "data_vigencia_inicio").toDate());

    if (record.contains(prefix + "data_vigencia_fim") && !record.isNull(prefix + "data_vigencia_fim"))
        n.setDataVigenciaFim(record.value(prefix + "data_vigencia_fim").toDate());

    if (record.contains(prefix + "criado_em") && !record.isNull(prefix + "criado_em"))
        n.setCriadoEm(record.value(prefix + "criado_em").toDateTime());

    if (record.contains(prefix + "atualizado_em") && !record.isNull(prefix + "atualizado_em"))
        n.setAtualizadoEm(record.value(prefix + "atualizado_em").toDateTime());

    return n;
}
