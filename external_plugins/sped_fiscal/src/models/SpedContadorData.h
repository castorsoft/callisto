#pragma once

#include <QString>
#include <QJsonObject>

struct SpedContadorData {
    QString nome;
    QString cpf;
    QString crc;
    QString cnpj;
    QString cep;
    QString logradouro;
    QString numero;
    QString complemento;
    QString bairro;
    QString telefone;
    QString email;
    QString codigoMunicipioIbge;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["nome"] = nome;
        obj["cpf"] = cpf;
        obj["crc"] = crc;
        obj["cnpj"] = cnpj;
        obj["cep"] = cep;
        obj["logradouro"] = logradouro;
        obj["numero"] = numero;
        obj["complemento"] = complemento;
        obj["bairro"] = bairro;
        obj["telefone"] = telefone;
        obj["email"] = email;
        obj["codigoMunicipioIbge"] = codigoMunicipioIbge;
        return obj;
    }

    static SpedContadorData fromJson(const QJsonObject &obj) {
        SpedContadorData c;
        c.nome = obj.value("nome").toString();
        c.cpf = obj.value("cpf").toString();
        c.crc = obj.value("crc").toString();
        c.cnpj = obj.value("cnpj").toString();
        c.cep = obj.value("cep").toString();
        c.logradouro = obj.value("logradouro").toString();
        c.numero = obj.value("numero").toString();
        c.complemento = obj.value("complemento").toString();
        c.bairro = obj.value("bairro").toString();
        c.telefone = obj.value("telefone").toString();
        c.email = obj.value("email").toString();
        c.codigoMunicipioIbge = obj.value("codigoMunicipioIbge").toString();
        return c;
    }
};

