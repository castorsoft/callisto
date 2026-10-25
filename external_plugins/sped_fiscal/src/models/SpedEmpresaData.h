#pragma once

#include <QString>

struct SpedEmpresaData {
    QString cnpjCpf;
    QString razaoSocial;
    QString nomeFantasia;
    QString inscricaoEstadual;
    QString codigoMunicipioIbge;
    QString suframa;
    QString cep;
    QString logradouro;
    QString numero;
    QString complemento;
    QString bairro;
    QString telefone;
    QString email;
    QString perfilSped{"A"}; // "A" ou "B"
    int indicadorAtividade{1}; // 0 = Industrial, 1 = Outros
};

