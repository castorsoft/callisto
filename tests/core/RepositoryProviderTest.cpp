#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/RepositoryProvider.h"
#include "core/interfaces/IRepositoryFactory.h"
#include "core/interfaces/IEmpresaRepository.h"
#include "core/interfaces/IPessoaRepository.h"
#include "core/interfaces/IProdutoRepository.h"
#include "core/interfaces/ICidadeRepository.h"
#include "core/interfaces/IEstadoRepository.h"
#include "core/interfaces/IUnidadeMedidaRepository.h"
#include "core/interfaces/INCMRepository.h"
#include "core/interfaces/IDocumentoFiscalRepository.h"
#include "core/interfaces/IDocumentoFiscalArquivoRepository.h"
#include "core/interfaces/IProdutoFornecedorRepository.h"
#include "core/interfaces/IEnderecoRepository.h"

class MockRepositoryFactory : public IRepositoryFactory {
public:
    MOCK_METHOD(ICidadeRepository&, cidadeRepository, (), (override));
    MOCK_METHOD(IDocumentoFiscalArquivoRepository&, documentoFiscalArquivoRepository, (), (override));
    MOCK_METHOD(IDocumentoFiscalRepository&, documentoFiscalRepository, (), (override));
    MOCK_METHOD(IEmpresaRepository&, empresaRepository, (), (override));
    MOCK_METHOD(IEnderecoRepository&, enderecoRepository, (), (override));
    MOCK_METHOD(IEstadoRepository&, estadoRepository, (), (override));
    MOCK_METHOD(INCMRepository&, ncmRepository, (), (override));
    MOCK_METHOD(IPessoaRepository&, pessoaRepository, (), (override));
    MOCK_METHOD(IProdutoFornecedorRepository&, produtoFornecedorRepository, (), (override));
    MOCK_METHOD(IProdutoRepository&, produtoRepository, (), (override));
    MOCK_METHOD(IUnidadeMedidaRepository&, unidadeMedidaRepository, (), (override));
};

class RepositoryProviderTest : public ::testing::Test {
protected:
    void SetUp() override {
        RepositoryProvider::reset();
    }

    void TearDown() override {
        RepositoryProvider::reset();
    }
};

TEST_F(RepositoryProviderTest, ThrowsWhenUninitialized) {
    EXPECT_FALSE(RepositoryProvider::isInitialized());
    EXPECT_THROW(RepositoryProvider::instance(), std::runtime_error);
    EXPECT_THROW(RepositoryProvider::empresaRepository(), std::runtime_error);
}

TEST_F(RepositoryProviderTest, SetFactoryAndAccessInstance) {
    auto mockFactory = std::make_shared<MockRepositoryFactory>();
    RepositoryProvider::setFactory(mockFactory);

    EXPECT_TRUE(RepositoryProvider::isInitialized());
    EXPECT_EQ(&RepositoryProvider::instance(), mockFactory.get());
}

