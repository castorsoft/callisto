#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "../../../core/interfaces/IDocumentoFiscalArquivoRepository.h"

class DocumentoFiscalArquivoRepository : public IDocumentoFiscalArquivoRepository {
public:
    DocumentoFiscalArquivoRepository();
    ~DocumentoFiscalArquivoRepository() override = default;

    bool salvar(DocumentoFiscalArquivo &arquivo) override;
    std::optional<DocumentoFiscalArquivo> buscarPorId(int id) override;
    std::optional<DocumentoFiscalArquivo> buscarPrincipalPorIdDocumento(int idDocumento) override;
    QList<DocumentoFiscalArquivo> listarPorIdDocumento(int idDocumento) override;
};
