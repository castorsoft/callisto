#pragma once

#include "../SpedRecordBase.h"

class RegistroC001 : public SpedRecordBase {
public:
    RegistroC001(int indMov = 0) : SpedRecordBase("C001"), indMov(indMov) {}

    int indMov{0}; // 0 = Bloco com dados informados, 1 = Bloco sem dados

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatInt(indMov);
        return assembleLine(fields);
    }
};

class RegistroC100 : public SpedRecordBase {
public:
    RegistroC100() : SpedRecordBase("C100") {}

    int indOper{0};        // 0 = Entrada, 1 = Saída
    int indEmit{0};        // 0 = Emissão própria, 1 = Terceiros
    QString codPart;
    QString codMod{"55"};  // Modelo NF-e
    QString codSit{"00"};  // 00 = Documento Regular
    QString ser{"1"};
    int numDoc{0};
    QString chvNfe;
    QDate dtDoc;
    QDate dtES;
    double vlDoc{0.0};
    int indPgto{0};        // 0 = À vista, 1 = A prazo, 9 = Sem pagamento
    double vlDesc{0.0};
    double vlAbatNt{0.0};
    double vlMerc{0.0};
    int indFrt{9};         // 9 = Sem ocorrência de transporte
    double vlFrt{0.0};
    double vlSeg{0.0};
    double vlOutDa{0.0};
    double vlBcIcms{0.0};
    double vlIcms{0.0};
    double vlBcIcmsSt{0.0};
    double vlIcmsSt{0.0};
    double vlIpi{0.0};
    double vlPis{0.0};
    double vlCofins{0.0};
    double vlPisSt{0.0};
    double vlCofinsSt{0.0};

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatInt(indOper);
        fields << formatInt(indEmit);
        fields << formatString(codPart);
        fields << formatString(codMod);
        fields << formatString(codSit);
        fields << formatString(ser);
        fields << formatInt(numDoc);
        fields << formatString(chvNfe);
        fields << formatDate(dtDoc);
        fields << formatDate(dtES);
        fields << formatDecimal(vlDoc);
        fields << formatInt(indPgto);
        fields << formatDecimal(vlDesc, 2, true);
        fields << formatDecimal(vlAbatNt, 2, true);
        fields << formatDecimal(vlMerc);
        fields << formatInt(indFrt);
        fields << formatDecimal(vlFrt, 2, true);
        fields << formatDecimal(vlSeg, 2, true);
        fields << formatDecimal(vlOutDa, 2, true);
        fields << formatDecimal(vlBcIcms, 2, true);
        fields << formatDecimal(vlIcms, 2, true);
        fields << formatDecimal(vlBcIcmsSt, 2, true);
        fields << formatDecimal(vlIcmsSt, 2, true);
        fields << formatDecimal(vlIpi, 2, true);
        fields << formatDecimal(vlPis, 2, true);
        fields << formatDecimal(vlCofins, 2, true);
        fields << formatDecimal(vlPisSt, 2, true);
        fields << formatDecimal(vlCofinsSt, 2, true);
        return assembleLine(fields);
    }
};

