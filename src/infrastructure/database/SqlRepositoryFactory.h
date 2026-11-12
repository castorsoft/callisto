#pragma once

#include "core/interfaces/IRepositoryFactory.h"
#include "repositories/CidadeRepository.h"
#include "repositories/DocumentoFiscalArquivoRepository.h"
#include "repositories/DocumentoFiscalRepository.h"
#include "repositories/EmpresaRepository.h"
#include "repositories/EnderecoRepository.h"
#include "repositories/EstadoRepository.h"
#include "repositories/NCMRepository.h"
#include "repositories/PessoaRepository.h"
#include "repositories/ProdutoFornecedorRepository.h"
#include "repositories/ProdutoRepository.h"
#include "repositories/UnidadeMedidaRepository.h"

class SqlRepositoryFactory : public IRepositoryFactory {
public:
    SqlRepositoryFactory() = default;
    ~SqlRepositoryFactory() override = default;

    ICidadeRepository& cidadeRepository() override { return m_cidadeRepo; }
    IDocumentoFiscalArquivoRepository& documentoFiscalArquivoRepository() override { return m_docFiscalArquivoRepo; }
    IDocumentoFiscalRepository& documentoFiscalRepository() override { return m_docFiscalRepo; }
    IEmpresaRepository& empresaRepository() override { return m_empresaRepo; }
    IEnderecoRepository& enderecoRepository() override { return m_enderecoRepo; }
    IEstadoRepository& estadoRepository() override { return m_estadoRepo; }
    INCMRepository& ncmRepository() override { return m_ncmRepo; }
    IPessoaRepository& pessoaRepository() override { return m_pessoaRepo; }
    IProdutoFornecedorRepository& produtoFornecedorRepository() override { return m_produtoFornecedorRepo; }
    IProdutoRepository& produtoRepository() override { return m_produtoRepo; }
    IUnidadeMedidaRepository& unidadeMedidaRepository() override { return m_unidadeMedidaRepo; }

private:
    CidadeRepository m_cidadeRepo;
    DocumentoFiscalArquivoRepository m_docFiscalArquivoRepo;
    DocumentoFiscalRepository m_docFiscalRepo;
    EmpresaRepository m_empresaRepo;
    EnderecoRepository m_enderecoRepo;
    EstadoRepository m_estadoRepo;
    NCMRepository m_ncmRepo;
    PessoaRepository m_pessoaRepo;
    ProdutoFornecedorRepository m_produtoFornecedorRepo;
    ProdutoRepository m_produtoRepo;
    UnidadeMedidaRepository m_unidadeMedidaRepo;
};

