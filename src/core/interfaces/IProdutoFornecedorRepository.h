#pragma once

#include <optional>
#include <QUuid>
#include <QList>
#include <QString>
#include "../entities/ProdutoFornecedor.h"

class IProdutoFornecedorRepository {
public:
    virtual ~IProdutoFornecedorRepository() = default;

    virtual bool salvar(ProdutoFornecedor &pf) = 0;
    virtual std::optional<ProdutoFornecedor> buscarPorId(int id) = 0;
    virtual std::optional<ProdutoFornecedor> buscarPorFornecedorECodigo(int idFornecedor, const QString &codigoFornecedor) = 0;
    virtual QList<ProdutoFornecedor> listarPorFornecedor(int idFornecedor) = 0;
    virtual QList<ProdutoFornecedor> listarPorProduto(int idProduto) = 0;
    virtual bool deletar(int id) = 0;
};

