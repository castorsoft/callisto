#pragma once

#include "../SpedRecordBase.h"

class Registro0000 : public SpedRecordBase {
public:
    Registro0000() : SpedRecordBase("0000") {}

    QString codVer{"018"}; // Versão do leiaute EFD ICMS/IPI
    int codFin{0};        // 0 = Original, 1 = Substituto
    QDate dtIni;
    QDate dtFin;
    QString nome;
    QString cnpj;
    QString cpf;
    QString uf;
    QString ie;
    QString codMun;
    QString im;
    QString suframa;
    QString indPerfil{"A"};
    int indAtiv{1};       // 0 = Industrial, 1 = Outros

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << codVer;
        fields << formatInt(codFin);
        fields << formatDate(dtIni);
        fields << formatDate(dtFin);
        fields << formatString(nome);
        fields << formatString(cnpj);
        fields << formatString(cpf);
        fields << formatString(uf);
        fields << formatString(ie);
        fields << formatString(codMun);
        fields << formatString(im);
        fields << formatString(suframa);
        fields << formatString(indPerfil);
        fields << formatInt(indAtiv);
        return assembleLine(fields);
    }
};

class Registro0001 : public SpedRecordBase {
public:
    Registro0001(int indMov = 0) : SpedRecordBase("0001"), indMov(indMov) {}

    int indMov{0}; // 0 = Bloco com dados informados, 1 = Bloco sem dados

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatInt(indMov);
        return assembleLine(fields);
    }
};

