#pragma once

#include <gmock/gmock.h>

#include "core/interfaces/ICidadeRepository.h"
#include "core/interfaces/IDocumentoFiscalArquivoRepository.h"
#include "core/interfaces/IDocumentoFiscalRepository.h"
#include "core/interfaces/IEmpresaRepository.h"
#include "core/interfaces/IEnderecoRepository.h"
#include "core/interfaces/IEstadoRepository.h"
#include "core/interfaces/INCMRepository.h"
#include "core/interfaces/IPessoaRepository.h"
#include "core/interfaces/IProdutoFornecedorRepository.h"
#include "core/interfaces/IProdutoRepository.h"
#include "core/interfaces/IUnidadeMedidaRepository.h"

class MockCidadeRepository : public ICidadeRepository {
public:
    MOCK_METHOD(QList<Cidade>, listarPorEstado, (int idEstado), (override));
    MOCK_METHOD(std::optional<Cidade>, buscarPorId, (int id), (override));
    MOCK_METHOD(std::optional<Cidade>, buscarPorIbge, (int idIbge), (override));
};

class MockDocumentoFiscalArquivoRepository : public IDocumentoFiscalArquivoRepository {
public:
    MOCK_METHOD(bool, salvar, (DocumentoFiscalArquivo &arquivo), (override));
    MOCK_METHOD(std::optional<DocumentoFiscalArquivo>, buscarPorId, (int id), (override));
    MOCK_METHOD(std::optional<DocumentoFiscalArquivo>, buscarPrincipalPorIdDocumento, (int idDocumento), (override));
    MOCK_METHOD(QList<DocumentoFiscalArquivo>, listarPorIdDocumento, (int idDocumento), (override));
};

class MockDocumentoFiscalRepository : public IDocumentoFiscalRepository {
public:
    MOCK_METHOD(bool, salvar, (DocumentoFiscal &documentoFiscal), (override));
    MOCK_METHOD(QList<DocumentoFiscal>, listarPorPeriodo, (const QDate &dataInicio, const QDate &dataFim, bool carregarItens), (override));
    MOCK_METHOD(std::optional<DocumentoFiscal>, buscarPorId, (int id, bool carregarItens), (override));
    MOCK_METHOD(std::optional<DocumentoFiscal>, buscarPorChave, (const QString &chaveAcesso, bool carregarItens), (override));
    MOCK_METHOD(bool, existeDocumento, (const QString &chaveAcesso), (override));
    MOCK_METHOD(QList<NotaTerceiroResumo>, listarNotasTerceirosResumo, (bool apenasPendentes), (override));
    MOCK_METHOD(QList<NotaTerceiroItem>, carregarItensNotaTerceiro, (int idDocumentoFiscal), (override));
};

class MockEmpresaRepository : public IEmpresaRepository {
public:
    MOCK_METHOD(void, salvar, (Empresa &modelEmpresa), (override));
    MOCK_METHOD(std::optional<Empresa>, buscarPorId, (int id), (override));
    MOCK_METHOD(std::optional<Empresa>, buscarPorUuid, (const QUuid &uuid), (override));
    MOCK_METHOD(std::optional<Empresa>, buscarPorCnpjCpf, (const QString &cnpjCpf), (override));
    MOCK_METHOD(QList<Empresa>, listarTodas, (), (override));
    MOCK_METHOD(bool, deletar, (int id), (override));
};

class MockEnderecoRepository : public IEnderecoRepository {
public:
    MOCK_METHOD(void, salvar, (Endereco &modelEndereco), (override));
    MOCK_METHOD(std::optional<Endereco>, buscarPorId, (int id), (override));
    MOCK_METHOD(std::optional<Endereco>, buscarPorUuid, (const QUuid &uuid), (override));
    MOCK_METHOD(bool, deletar, (int id), (override));
};

class MockEstadoRepository : public IEstadoRepository {
public:
    MOCK_METHOD(QList<Estado>, listarTodos, (), (override));
};

class MockNCMRepository : public INCMRepository {
public:
    MOCK_METHOD(bool, salvar, (NCM &ncm), (override));
    MOCK_METHOD(std::optional<NCM>, buscarPorId, (int id), (override));
    MOCK_METHOD(std::optional<NCM>, buscarPorCodigo, (const QString &codigo), (override));
    MOCK_METHOD(QList<NCM>, buscarPorTermo, (const QString &termo), (override));
};

class MockPessoaRepository : public IPessoaRepository {
public:
    MOCK_METHOD(bool, salvar, (Pessoa &modelPessoa), (override));
    MOCK_METHOD(std::optional<Pessoa>, buscarPorId, (int id), (override));
    MOCK_METHOD(std::optional<Pessoa>, buscarPorUuid, (const QUuid &uuid), (override));
    MOCK_METHOD(std::optional<Pessoa>, buscarPorCnpjCpf, (const QString &cnpjCpf), (override));
    MOCK_METHOD(QList<Pessoa>, buscarContadorPorTermo, (const QString &termo), (override));
    MOCK_METHOD(QList<Pessoa>, listarTodas, (), (override));
    MOCK_METHOD(bool, deletar, (int id), (override));
};

class MockProdutoFornecedorRepository : public IProdutoFornecedorRepository {
public:
    MOCK_METHOD(bool, salvar, (ProdutoFornecedor &pf), (override));
    MOCK_METHOD(std::optional<ProdutoFornecedor>, buscarPorId, (int id), (override));
    MOCK_METHOD(std::optional<ProdutoFornecedor>, buscarPorFornecedorECodigo, (int idFornecedor, const QString &codigoFornecedor), (override));
    MOCK_METHOD(QList<ProdutoFornecedor>, listarPorFornecedor, (int idFornecedor), (override));
    MOCK_METHOD(QList<ProdutoFornecedor>, listarPorProduto, (int idProduto), (override));
    MOCK_METHOD(bool, deletar, (int id), (override));
};

class MockProdutoRepository : public IProdutoRepository {
public:
    MOCK_METHOD(bool, salvar, (Produto &produto), (override));
    MOCK_METHOD(std::optional<Produto>, buscarPorId, (int id), (override));
    MOCK_METHOD(std::optional<Produto>, buscarPorUuid, (const QUuid &uuid), (override));
    MOCK_METHOD(std::optional<Produto>, buscarPorCodigoInterno, (const QString &codigoInterno), (override));
    MOCK_METHOD(std::optional<Produto>, buscarPorCodigoBarra, (const QString &codigoBarra), (override));
    MOCK_METHOD(std::optional<Produto>, buscarPorDescricao, (const QString &descricao), (override));
    MOCK_METHOD(QList<Produto>, listarTodos, (), (override));
    MOCK_METHOD(bool, deletar, (int id), (override));
};

class MockUnidadeMedidaRepository : public IUnidadeMedidaRepository {
public:
    MOCK_METHOD(bool, salvar, (UnidadeMedida &unidadeMedida), (override));
    MOCK_METHOD(std::optional<UnidadeMedida>, buscarPorId, (int id), (override));
    MOCK_METHOD(std::optional<UnidadeMedida>, buscarPorSigla, (const QString &sigla), (override));
    MOCK_METHOD(QList<UnidadeMedida>, buscarPorTermo, (const QString &termo), (override));
    MOCK_METHOD(QList<UnidadeMedida>, listarTodas, (), (override));
};

