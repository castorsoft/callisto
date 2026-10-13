#pragma once

#include "../../../core/interfaces/IDocumentoFiscalRepository.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DocumentoFiscalRepository : public IDocumentoFiscalRepository {
public:
    DocumentoFiscalRepository();
    ~DocumentoFiscalRepository() override = default;

    bool salvar(DocumentoFiscal& documentoFiscal) override;
    std::optional<DocumentoFiscal> buscarPorId(int id, bool carregarItens = false) override;
    std::optional<DocumentoFiscal> buscarPorChave(const QString& chaveAcesso, bool carregarItens = false) override;
    bool existeDocumento(const QString& chaveAcesso) override;

    QList<NotaTerceiroResumo> listarNotasTerceirosResumo(bool apenasPendentes = true) override;
    QList<ItemNotaTerceiroInfo> carregarItensNotaTerceiro(int idDocumentoFiscal) override;

private:
    bool inserirItens(const std::vector<DocumentoFiscalItem>& itens, int idDocumentoFiscal, QSqlDatabase& db);
    bool inserirTributacoesItem(const DocumentoFiscalItem& item, int idItem, QSqlDatabase& db);
};
