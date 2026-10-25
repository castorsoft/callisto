#pragma once

#include <QString>

struct SpedItemData {
    QString codigoItem;
    QString descricao;
    QString codigoBarra;
    QString unidadeMedida;
    QString tipoItem{"00"}; // 00 = Mercadoria para Revenda
    QString ncm;
    double aliquotaIcms{0.0};
};

