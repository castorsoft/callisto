#pragma once

#include <optional>
#include <QUuid>
#include <QList>
#include "../../../core/interfaces/IProdutoRepository.h"
#include "../../../core/entities/Produto.h"

class ProdutoRepository : public IProdutoRepository {
public:
    ProdutoRepository() = default;
    ~ProdutoRepository() override = default;

    bool salvar(Produto &produto) override;
    std::optional<Produto> buscarPorId(int id) override;
    std::optional<Produto> buscarPorUuid(const QUuid &uuid) override;
    std::optional<Produto> buscarPorCodigoInterno(const QString &codigoInterno) override;
    std::optional<Produto> buscarPorCodigoBarra(const QString &codigoBarra) override;
    std::optional<Produto> buscarPorDescricao(const QString &descricao) override;
    QList<Produto> listarTodos() override;
    bool deletar(int id) override;

private:
};
