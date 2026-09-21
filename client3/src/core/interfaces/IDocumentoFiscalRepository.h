#pragma once

#include <QString>
#include <vector>
#include <optional>
#include "../entities/DocumentoFiscal.h"

class IDocumentoFiscalRepository {
public:
    virtual ~IDocumentoFiscalRepository() = default;

    // Salva o documento fiscal e seus itens no banco de dados, utilizando transação
    // Retorna true em caso de sucesso e preenche o m_id através do método hidratar.
    virtual bool salvar(DocumentoFiscal& documentoFiscal) = 0;

    // Busca um documento pelo ID, opcionalmente carregando também os itens
    virtual std::optional<DocumentoFiscal> buscarPorId(int id, bool carregarItens = false) = 0;

    // Busca um documento pela chave de acesso
    virtual std::optional<DocumentoFiscal> buscarPorChave(const QString& chaveAcesso, bool carregarItens = false) = 0;

    // Verifica se já existe um documento com essa chave de acesso no banco
    virtual bool existeDocumento(const QString& chaveAcesso) = 0;
};
