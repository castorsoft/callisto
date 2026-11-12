#include "RepositoryProvider.h"
#include <stdexcept>

std::shared_ptr<IRepositoryFactory> RepositoryProvider::s_factory = nullptr;

void RepositoryProvider::setFactory(std::shared_ptr<IRepositoryFactory> factory)
{
    s_factory = std::move(factory);
}

IRepositoryFactory& RepositoryProvider::instance()
{
    if (!s_factory) {
        throw std::runtime_error("RepositoryProvider::instance() chamado sem factory registrada.");
    }
    return *s_factory;
}

bool RepositoryProvider::isInitialized()
{
    return s_factory != nullptr;
}

void RepositoryProvider::reset()
{
    s_factory.reset();
}

ICidadeRepository& RepositoryProvider::cidadeRepository()
{
    return instance().cidadeRepository();
}

IDocumentoFiscalArquivoRepository& RepositoryProvider::documentoFiscalArquivoRepository()
{
    return instance().documentoFiscalArquivoRepository();
}

IDocumentoFiscalRepository& RepositoryProvider::documentoFiscalRepository()
{
    return instance().documentoFiscalRepository();
}

IEmpresaRepository& RepositoryProvider::empresaRepository()
{
    return instance().empresaRepository();
}

IEnderecoRepository& RepositoryProvider::enderecoRepository()
{
    return instance().enderecoRepository();
}

IEstadoRepository& RepositoryProvider::estadoRepository()
{
    return instance().estadoRepository();
}

INCMRepository& RepositoryProvider::ncmRepository()
{
    return instance().ncmRepository();
}

IPessoaRepository& RepositoryProvider::pessoaRepository()
{
    return instance().pessoaRepository();
}

IProdutoFornecedorRepository& RepositoryProvider::produtoFornecedorRepository()
{
    return instance().produtoFornecedorRepository();
}

IProdutoRepository& RepositoryProvider::produtoRepository()
{
    return instance().produtoRepository();
}

IUnidadeMedidaRepository& RepositoryProvider::unidadeMedidaRepository()
{
    return instance().unidadeMedidaRepository();
}

