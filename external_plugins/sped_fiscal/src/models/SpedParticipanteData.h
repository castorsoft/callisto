#pragma once

#include <QString>

struct SpedParticipanteData {
    QString codigoParticipante; // ID único do participante (ex: "FORN_10", "CLI_25" ou ID)
    QString nome;
    QString cnpjCpf;
    QString inscricaoEstadual;
    QString codigoMunicipioIbge;
    QString suframa;
    QString logradouro;
    QString numero;
    QString complemento;
    QString bairro;
    QString cep;
    QString telefone;
};

