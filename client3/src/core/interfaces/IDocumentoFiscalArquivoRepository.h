#pragma once

#include <optional>
#include <QList>
#include "../entities/DocumentoFiscalArquivo.h"

class IDocumentoFiscalArquivoRepository {
public:
    virtual ~IDocumentoFiscalArquivoRepository() = default;

    virtual bool salvar(DocumentoFiscalArquivo &arquivo) = 0;
    virtual std::optional<DocumentoFiscalArquivo> buscarPorId(int id) = 0;
    virtual std::optional<DocumentoFiscalArquivo> buscarPrincipalPorIdDocumento(int idDocumento) = 0;
    virtual QList<DocumentoFiscalArquivo> listarPorIdDocumento(int idDocumento) = 0;
};
