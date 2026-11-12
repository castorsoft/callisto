#pragma once

#include <QDate>
#include <QList>
#include <memory>
#include "../../../src/core/plugins/IPluginContext.h"
#include "../models/SpedEmpresaData.h"
#include "../models/SpedContadorData.h"
#include "../models/SpedParticipanteData.h"
#include "../models/SpedItemData.h"
#include "../models/SpedUnidadeData.h"
#include "../models/SpedDocumentoFiscalData.h"

struct SpedCollectedData {
    SpedEmpresaData empresa;
    SpedContadorData contador;
    QList<SpedParticipanteData> participantes;
    QList<SpedUnidadeData> unidades;
    QList<SpedItemData> itens;
    QList<SpedDocumentoFiscalData> documentos;
    QDate dataInicio;
    QDate dataFim;
    int finalidade{0}; // 0 = Remessa Original, 1 = Remessa Substituta
    QString perfil{"A"};
};

class SpedDataCollector {
public:
    static SpedCollectedData collect(
        IPluginContext &context,
        const QDate &dataInicio,
        const QDate &dataFim,
        int finalidade,
        const QString &perfil,
        std::optional<int> idContador = std::nullopt
    );
};
