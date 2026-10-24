#pragma once

#include <QString>

struct SpedDocumentoItemData {
    int numeroItem{1};
    QString codigoItem;
    QString descricaoComplementar;
    double quantidade{0.0};
    QString unidadeMedida;
    double valorTotal{0.0};
    double valorDesconto{0.0};
    int indicadorMovimentacaoFisica{0}; // 0 = Sim

    // Tributação ICMS
    QString cstIcms{"000"};
    QString cfop{"5102"};
    double baseCalculoIcms{0.0};
    double aliquotaIcms{0.0};
    double valorIcms{0.0};

    // ICMS ST
    double baseCalculoIcmsSt{0.0};
    double aliquotaIcmsSt{0.0};
    double valorIcmsSt{0.0};

    // IPI
    QString cstIpi;
    double baseCalculoIpi{0.0};
    double aliquotaIpi{0.0};
    double valorIpi{0.0};

    // PIS
    QString cstPis;
    double baseCalculoPis{0.0};
    double aliquotaPis{0.0};
    double valorPis{0.0};

    // COFINS
    QString cstCofins;
    double baseCalculoCofins{0.0};
    double aliquotaCofins{0.0};
    double valorCofins{0.0};
};

