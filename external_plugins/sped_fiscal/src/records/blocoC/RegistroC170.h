#pragma once

#include "../SpedRecordBase.h"

class RegistroC170 : public SpedRecordBase {
public:
    RegistroC170() : SpedRecordBase("C170") {}

    int numItem{1};
    QString codItem;
    QString descrCompl;
    double qtd{0.0};
    QString unid;
    double vlItem{0.0};
    double vlDesc{0.0};
    int indMov{0};         // 0 = Movimentação física ocorrida
    QString cstIcms{"000"};
    QString cfop{"5102"};
    QString codNat;
    double vlBcIcms{0.0};
    double aliqIcms{0.0};
    double vlIcms{0.0};
    double vlBcIcmsSt{0.0};
    double aliqSt{0.0};
    double vlIcmsSt{0.0};
    int indApur{0};        // 0 = Apuração própria do IPI
    QString cstIpi;
    QString codEnq;
    double vlBcIpi{0.0};
    double aliqIpi{0.0};
    double vlIpi{0.0};
    QString cstPis;
    double vlBcPis{0.0};
    double aliqPis{0.0};
    double quantBcPis{0.0};
    double aliqPisQuant{0.0};
    double vlPis{0.0};
    QString cstCofins;
    double vlBcCofins{0.0};
    double aliqCofins{0.0};
    double quantBcCofins{0.0};
    double aliqCofinsQuant{0.0};
    double vlCofins{0.0};
    QString codCta;

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatInt(numItem);
        fields << formatString(codItem);
        fields << formatString(descrCompl);
        fields << formatDecimal(qtd, 5);
        fields << formatString(unid);
        fields << formatDecimal(vlItem);
        fields << formatDecimal(vlDesc, 2, true);
        fields << formatInt(indMov);
        fields << formatString(cstIcms);
        fields << formatString(cfop);
        fields << formatString(codNat);
        fields << formatDecimal(vlBcIcms, 2, true);
        fields << formatDecimal(aliqIcms, 2, true);
        fields << formatDecimal(vlIcms, 2, true);
        fields << formatDecimal(vlBcIcmsSt, 2, true);
        fields << formatDecimal(aliqSt, 2, true);
        fields << formatDecimal(vlIcmsSt, 2, true);
        fields << formatInt(indApur);
        fields << formatString(cstIpi);
        fields << formatString(codEnq);
        fields << formatDecimal(vlBcIpi, 2, true);
        fields << formatDecimal(aliqIpi, 2, true);
        fields << formatDecimal(vlIpi, 2, true);
        fields << formatString(cstPis);
        fields << formatDecimal(vlBcPis, 2, true);
        fields << formatDecimal(aliqPis, 2, true);
        fields << formatDecimal(quantBcPis, 3, true);
        fields << formatDecimal(aliqPisQuant, 4, true);
        fields << formatDecimal(vlPis, 2, true);
        fields << formatString(cstCofins);
        fields << formatDecimal(vlBcCofins, 2, true);
        fields << formatDecimal(aliqCofins, 2, true);
        fields << formatDecimal(quantBcCofins, 3, true);
        fields << formatDecimal(aliqCofinsQuant, 4, true);
        fields << formatDecimal(vlCofins, 2, true);
        fields << formatString(codCta);
        return assembleLine(fields);
    }
};

class RegistroC190 : public SpedRecordBase {
public:
    RegistroC190() : SpedRecordBase("C190") {}

    QString cstIcms;
    QString cfop;
    double aliqIcms{0.0};
    double vlOpr{0.0};
    double vlBcIcms{0.0};
    double vlIcms{0.0};
    double vlBcIcmsSt{0.0};
    double vlIcmsSt{0.0};
    double vlRedBc{0.0};
    double vlIpi{0.0};
    QString codObs;

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatString(cstIcms);
        fields << formatString(cfop);
        fields << formatDecimal(aliqIcms, 2, true);
        fields << formatDecimal(vlOpr);
        fields << formatDecimal(vlBcIcms, 2, true);
        fields << formatDecimal(vlIcms, 2, true);
        fields << formatDecimal(vlBcIcmsSt, 2, true);
        fields << formatDecimal(vlIcmsSt, 2, true);
        fields << formatDecimal(vlRedBc, 2, true);
        fields << formatDecimal(vlIpi, 2, true);
        fields << formatString(codObs);
        return assembleLine(fields);
    }
};

class RegistroC990 : public SpedRecordBase {
public:
    RegistroC990(int qtdLin = 0) : SpedRecordBase("C990"), qtdLin(qtdLin) {}

    int qtdLin{0};

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatInt(qtdLin);
        return assembleLine(fields);
    }
};

