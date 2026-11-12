#pragma once

#include "ICidadeRepository.h"
#include "IDocumentoFiscalArquivoRepository.h"
#include "IDocumentoFiscalRepository.h"
#include "IEmpresaRepository.h"
#include "IEnderecoRepository.h"
#include "IEstadoRepository.h"
#include "INCMRepository.h"
#include "IPessoaRepository.h"
#include "IProdutoFornecedorRepository.h"
#include "IProdutoRepository.h"
#include "IUnidadeMedidaRepository.h"

class IRepositoryFactory {
public:
    virtual ~IRepositoryFactory() = default;

    virtual ICidadeRepository& cidadeRepository() = 0;
    virtual IDocumentoFiscalArquivoRepository& documentoFiscalArquivoRepository() = 0;
    virtual IDocumentoFiscalRepository& documentoFiscalRepository() = 0;
    virtual IEmpresaRepository& empresaRepository() = 0;
    virtual IEnderecoRepository& enderecoRepository() = 0;
    virtual IEstadoRepository& estadoRepository() = 0;
    virtual INCMRepository& ncmRepository() = 0;
    virtual IPessoaRepository& pessoaRepository() = 0;
    virtual IProdutoFornecedorRepository& produtoFornecedorRepository() = 0;
    virtual IProdutoRepository& produtoRepository() = 0;
    virtual IUnidadeMedidaRepository& unidadeMedidaRepository() = 0;
};
