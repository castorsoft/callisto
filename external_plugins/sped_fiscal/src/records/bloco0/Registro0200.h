#pragma once

#include "../SpedRecordBase.h"

class Registro0150 : public SpedRecordBase {
public:
    Registro0150() : SpedRecordBase("0150") {}

    QString codPart;
    QString nome;
    QString codPais{"01058"}; // Brasil
    QString cnpj;
    QString cpf;
    QString ie;
    QString codMun;
    QString suframa;
    QString endereco;
    QString num;
    QString complemento;
    QString bairro;

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatString(codPart);
        fields << formatString(nome);
        fields << formatString(codPais);
        fields << formatString(cnpj);
        fields << formatString(cpf);
        fields << formatString(ie);
        fields << formatString(codMun);
        fields << formatString(suframa);
        fields << formatString(endereco);
        fields << formatString(num);
        fields << formatString(complemento);
        fields << formatString(bairro);
        return assembleLine(fields);
    }
};

class Registro0190 : public SpedRecordBase {
public:
    Registro0190() : SpedRecordBase("0190") {}

    QString unid;
    QString descr;

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatString(unid);
        fields << formatString(descr);
        return assembleLine(fields);
    }
};

class Registro0200 : public SpedRecordBase {
public:
    Registro0200() : SpedRecordBase("0200") {}

    QString codItem;
    QString descrItem;
    QString codBarra;
    QString codAntItem;
    QString unidInv;
    QString tipoItem{"00"}; // 00 = Mercadoria para Revenda
    QString codNcm;
    QString exIpi;
    QString codGen;
    QString codLst;
    double aliqIcms{0.0};
    QString cest;

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatString(codItem);
        fields << formatString(descrItem);
        fields << formatString(codBarra);
        fields << formatString(codAntItem);
        fields << formatString(unidInv);
        fields << formatString(tipoItem);
        fields << formatString(codNcm);
        fields << formatString(exIpi);
        fields << formatString(codGen);
        fields << formatString(codLst);
        fields << formatDecimal(aliqIcms, 2, true);
        fields << formatString(cest);
        return assembleLine(fields);
    }
};

class Registro0990 : public SpedRecordBase {
public:
    Registro0990(int qtdLin = 0) : SpedRecordBase("0990"), qtdLin(qtdLin) {}

    int qtdLin{0};

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatInt(qtdLin);
        return assembleLine(fields);
    }
};
