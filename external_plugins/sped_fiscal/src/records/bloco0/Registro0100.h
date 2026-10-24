#pragma once

#include "../SpedRecordBase.h"

class Registro0005 : public SpedRecordBase {
public:
    Registro0005() : SpedRecordBase("0005") {}

    QString fantasia;
    QString cep;
    QString endereco;
    QString num;
    QString complemento;
    QString bairro;
    QString fone;
    QString fax;
    QString email;

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatString(fantasia);
        fields << formatString(cep);
        fields << formatString(endereco);
        fields << formatString(num);
        fields << formatString(complemento);
        fields << formatString(bairro);
        fields << formatString(fone);
        fields << formatString(fax);
        fields << formatString(email);
        return assembleLine(fields);
    }
};

class Registro0100 : public SpedRecordBase {
public:
    Registro0100() : SpedRecordBase("0100") {}

    QString nome;
    QString cpf;
    QString crc;
    QString cnpj;
    QString cep;
    QString endereco;
    QString num;
    QString complemento;
    QString bairro;
    QString fone;
    QString fax;
    QString email;
    QString codMun;

    QString buildLine() const override {
        QStringList fields;
        fields << m_regName;
        fields << formatString(nome);
        fields << formatString(cpf);
        fields << formatString(crc);
        fields << formatString(cnpj);
        fields << formatString(cep);
        fields << formatString(endereco);
        fields << formatString(num);
        fields << formatString(complemento);
        fields << formatString(bairro);
        fields << formatString(fone);
        fields << formatString(fax);
        fields << formatString(email);
        fields << formatString(codMun);
        return assembleLine(fields);
    }
};
