#pragma once

#include <QString>
#include <QDate>
#include <vector>
#include "SpedDocumentoItemData.h"

struct SpedDocumentoFiscalData {
    int indicadorOperacao{0};   // 0 = Entrada, 1 = Saída
    int indicadorEmissao{0};    // 0 = Emissão própria, 1 = Terceiros
    QString codigoParticipante;
    QString codigoModelo{"55"}; // 55 = NF-e
    QString codigoSituacao{"00"}; // 00 = Regular
    QString serie{"1"};
    int numero{0};
    QString chaveAcesso;
    QDate dataEmissao;
    QDate dataEntradaSaida;
    double valorTotalDocumento{0.0};
    int indicadorPagamento{0};  // 0 = À vista, 1 = A prazo, 9 = Sem pagamento
    double valorDesconto{0.0};
    double valorAbatimento{0.0};
    double valorMercadorias{0.0};
    int indicadorFrete{9};      // 9 = Sem frete
    double valorFrete{0.0};
    double valorSeguro{0.0};
    double outrasDespesas{0.0};
    double baseCalculoIcms{0.0};
    double valorIcms{0.0};
    double baseCalculoIcmsSt{0.0};
    double valorIcmsSt{0.0};
    double valorIpi{0.0};
    double valorPis{0.0};
    double valorCofins{0.0};

    std::vector<SpedDocumentoItemData> itens;
};

