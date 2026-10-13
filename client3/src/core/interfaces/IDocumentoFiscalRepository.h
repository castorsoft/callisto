#pragma once

#include <QString>
#include <vector>
#include <optional>
#include "../entities/DocumentoFiscal.h"

struct NotaTerceiroResumo {
    int idDocumentoFiscal{0};
    QString chaveAcesso;
    QString modelo;
    QString serie;
    int numero{0};
    QDateTime dataEmissao;
    int idFornecedor{0};
    QString fornecedorNome;
    QString fornecedorCnpj;
    int totalItens{0};
    int itensPendentes{0};
};

struct ItemNotaTerceiroInfo {
    int idItem{0};
    int idDocumentoFiscal{0};
    int numeroItem{0};
    QString codigoProduto;
    QString codigoBarras;
    QString descricao;
    QString unidadeMedida;
    double quantidade{0.0};
    double valorUnitario{0.0};
    double valorTotal{0.0};

    bool vinculado{false};
    std::optional<int> idProdutoFornecedor;
    std::optional<int> idProduto;
    QString produtoCodigoInterno;
    QString produtoDescricao;
    double multiplicador{1.0};
    std::optional<int> idUnidadeMedidaInterna;
    QString siglaUnidadeMedidaInterna;
};

class IDocumentoFiscalRepository {
public:
    virtual ~IDocumentoFiscalRepository() = default;

    // Salva o documento fiscal e seus itens no banco de dados, utilizando transação
    // Retorna true em caso de sucesso e preenche o m_id.
    virtual bool salvar(DocumentoFiscal& documentoFiscal) = 0;

    // Busca um documento pelo ID, opcionalmente carregando também os itens
    virtual std::optional<DocumentoFiscal> buscarPorId(int id, bool carregarItens = false) = 0;

    // Busca um documento pela chave de acesso
    virtual std::optional<DocumentoFiscal> buscarPorChave(const QString& chaveAcesso, bool carregarItens = false) = 0;

    // Verifica se já existe um documento com essa chave de acesso no banco
    virtual bool existeDocumento(const QString& chaveAcesso) = 0;

    // Listagem de notas de terceiros com resumo de itens e contagem de pendências
    virtual QList<NotaTerceiroResumo> listarNotasTerceirosResumo(bool apenasPendentes = true) = 0;

    // Carrega itens de uma nota de terceiro detalhando o status de vínculo com produtos
    virtual QList<ItemNotaTerceiroInfo> carregarItensNotaTerceiro(int idDocumentoFiscal) = 0;
};
