#pragma once

#include "../SpedRecordBase.h"

// Registros de abertura de bloco sem movimento (IND_MOV = 1)
// Ex: D001, E001, G001, H001, 1001
class RegistroAberturaVazio : public SpedRecordBase {
public:
    explicit RegistroAberturaVazio(const QString &regName) : SpedRecordBase(regName) {}

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << "1"; // 1 = Bloco sem dados informados
        return assembleLine(fields);
    }
};

// Registros de encerramento de bloco
// Ex: D990, E990, G990, H990, 1990
class RegistroEncerramentoVazio : public SpedRecordBase {
public:
    explicit RegistroEncerramentoVazio(const QString &regName, int qtdLin = 2)
        : SpedRecordBase(regName), qtdLin(qtdLin) {}

    int qtdLin{2};

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatInt(qtdLin);
        return assembleLine(fields);
    }
};

