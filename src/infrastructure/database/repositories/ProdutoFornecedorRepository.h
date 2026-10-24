#pragma once

#include <optional>
#include <QUuid>
#include <QList>
#include "../../../core/interfaces/IProdutoFornecedorRepository.h"
#include "../../../core/entities/ProdutoFornecedor.h"

class ProdutoFornecedorRepository : public IProdutoFornecedorRepository {
public:
    ProdutoFornecedorRepository() = default;
    ~ProdutoFornecedorRepository() override = default;

    bool salvar(ProdutoFornecedor &pf) override;
    std::optional<ProdutoFornecedor> buscarPorId(int id) override;
    std::optional<ProdutoFornecedor> buscarPorFornecedorECodigo(int idFornecedor, const QString &codigoFornecedor) override;
    QList<ProdutoFornecedor> listarPorFornecedor(int idFornecedor) override;
    QList<ProdutoFornecedor> listarPorProduto(int idProduto) override;
    bool deletar(int id) override;
};

