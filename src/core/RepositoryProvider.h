#pragma once

#include "interfaces/IRepositoryFactory.h"
#include <memory>

class RepositoryProvider {
public:
    static void setFactory(std::shared_ptr<IRepositoryFactory> factory);
    static IRepositoryFactory& instance();
    static bool isInitialized();
    static void reset();

    // Métodos de conveniência diretos
    static ICidadeRepository& cidadeRepository();
    static IDocumentoFiscalArquivoRepository& documentoFiscalArquivoRepository();
    static IDocumentoFiscalRepository& documentoFiscalRepository();
    static IEmpresaRepository& empresaRepository();
    static IEnderecoRepository& enderecoRepository();
    static IEstadoRepository& estadoRepository();
    static INCMRepository& ncmRepository();
    static IPessoaRepository& pessoaRepository();
    static IProdutoFornecedorRepository& produtoFornecedorRepository();
    static IProdutoRepository& produtoRepository();
    static IUnidadeMedidaRepository& unidadeMedidaRepository();

private:
    static std::shared_ptr<IRepositoryFactory> s_factory;
};

