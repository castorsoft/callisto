#pragma once

#include <QString>
#include <QList>
#include <QDate>
#include <vector>
#include <optional>
#include "../entities/DocumentoFiscal.h"
#include "../entities/virtual/NotaTerceiroResumo.h"
#include "../entities/virtual/NotaTerceiroItem.h"

class IDocumentoFiscalRepository {
public:
    virtual ~IDocumentoFiscalRepository() = default;

    // Salva o documento fiscal e seus itens no banco de dados, utilizando transação
    // Retorna true em caso de sucesso e preenche o m_id.
    virtual bool salvar(DocumentoFiscal& documentoFiscal) = 0;

    // Busca documentos fiscais emitidos/recebidos em um período (por data de emissão)
    virtual QList<DocumentoFiscal> listarPorPeriodo(const QDate &dataInicio, const QDate &dataFim, bool carregarItens = true) = 0;

    // Busca um documento pelo ID, opcionalmente carregando também os itens
    virtual std::optional<DocumentoFiscal> buscarPorId(int id, bool carregarItens = false) = 0;

    // Busca um documento pela chave de acesso
    virtual std::optional<DocumentoFiscal> buscarPorChave(const QString& chaveAcesso, bool carregarItens = false) = 0;

    // Verifica se já existe um documento com essa chave de acesso no banco
    virtual bool existeDocumento(const QString& chaveAcesso) = 0;

    // Listagem de notas de terceiros com resumo de itens e contagem de pendências
    virtual QList<NotaTerceiroResumo> listarNotasTerceirosResumo(bool apenasPendentes = true) = 0;

    // Carrega itens de uma nota de terceiro detalhando o status de vínculo com produtos
    virtual QList<NotaTerceiroItem> carregarItensNotaTerceiro(int idDocumentoFiscal) = 0;
};
