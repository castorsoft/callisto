#pragma once

#include "../SpedRecordBase.h"

class Registro9001 : public SpedRecordBase {
public:
    Registro9001(int indMov = 0) : SpedRecordBase("9001"), indMov(indMov) {}

    int indMov{0};

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatInt(indMov);
        return assembleLine(fields);
    }
};

class Registro9900 : public SpedRecordBase {
public:
    Registro9900(const QString &regBlc = QString(), int qtdRegBlc = 0)
        : SpedRecordBase("9900"), regBlc(regBlc), qtdRegBlc(qtdRegBlc) {}

    QString regBlc;
    int qtdRegBlc{0};

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatString(regBlc);
        fields << formatInt(qtdRegBlc);
        return assembleLine(fields);
    }
};

class Registro9990 : public SpedRecordBase {
public:
    Registro9990(int qtdLin = 0) : SpedRecordBase("9990"), qtdLin(qtdLin) {}

    int qtdLin{0};

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatInt(qtdLin);
        return assembleLine(fields);
    }
};

class Registro9999 : public SpedRecordBase {
public:
    Registro9999(int qtdLin = 0) : SpedRecordBase("9999"), qtdLin(qtdLin) {}

    int qtdLin{0};

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatInt(qtdLin);
        return assembleLine(fields);
    }
};

