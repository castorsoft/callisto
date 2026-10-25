#pragma once

#include <optional>
#include <QUuid>
#include <QList>
#include "../entities/Produto.h"

class IProdutoRepository {
public:
    virtual ~IProdutoRepository() = default;

    virtual bool salvar(Produto &produto) = 0;
    virtual std::optional<Produto> buscarPorId(int id) = 0;
    virtual std::optional<Produto> buscarPorUuid(const QUuid &uuid) = 0;
    virtual std::optional<Produto> buscarPorCodigoInterno(const QString &codigoInterno) = 0;
    virtual std::optional<Produto> buscarPorCodigoBarra(const QString &codigoBarra) = 0;
    virtual std::optional<Produto> buscarPorDescricao(const QString &descricao) = 0;
    virtual QList<Produto> listarTodos() = 0;
    virtual bool deletar(int id) = 0;
};
