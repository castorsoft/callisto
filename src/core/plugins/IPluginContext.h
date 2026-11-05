#pragma once

#include <QString>
#include <memory>
#include <optional>
#include "../entities/Empresa.h"
#include "../interfaces/IEmpresaRepository.h"
#include "../interfaces/IDocumentoFiscalRepository.h"
#include "../interfaces/IPessoaRepository.h"
#include "../interfaces/IProdutoRepository.h"
#include "../interfaces/IProdutoFornecedorRepository.h"
#include "../interfaces/IUnidadeMedidaRepository.h"

class IPluginContext {
public:
    virtual ~IPluginContext() = default;

    // Estado da Aplicação
    virtual std::optional<Empresa> empresaAtual() const = 0;

    // Acesso aos Repositórios do Core
    virtual IEmpresaRepository& empresaRepository() const = 0;
    virtual IDocumentoFiscalRepository& documentoFiscalRepository() const = 0;
    virtual IPessoaRepository& pessoaRepository() const = 0;
    virtual IProdutoRepository& produtoRepository() const = 0;
    virtual IProdutoFornecedorRepository& produtoFornecedorRepository() const = 0;
    virtual IUnidadeMedidaRepository& unidadeMedidaRepository() const = 0;
};

