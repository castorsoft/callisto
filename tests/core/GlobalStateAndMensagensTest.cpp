#include <gtest/gtest.h>
#include <QSignalSpy>

#include "core/GlobalState.h"
#include "core/constants/Mensagens.h"
#include "core/entities/Empresa.h"
#include "core/entities/Endereco.h"

TEST(GlobalStateTest, SetAndClearEmpresaAtualEmitsSignal) {
    auto &state = GlobalState::instance();
    state.clearEmpresaAtual();
    EXPECT_FALSE(state.empresaAtual().has_value());

    QSignalSpy spy(&state, &GlobalState::empresaAtualChanged);

    Endereco end("Av. Brasil", "500", std::nullopt, "Centro", "85801000", 1);
    Empresa emp("12345678000199", "Empresa Ativa Ltda", "Fantasia", "Lucro Presumido", end, "123456789", std::nullopt);
    emp.setId(10);

    state.setEmpresaAtual(emp);

    ASSERT_TRUE(state.empresaAtual().has_value());
    EXPECT_EQ(state.empresaAtual()->id().value(), 10);
    EXPECT_EQ(state.empresaAtual()->cnpjCpf(), "12345678000199");
    EXPECT_EQ(state.empresaAtual()->razaoSocial(), "Empresa Ativa Ltda");
    EXPECT_EQ(spy.count(), 1);

    state.clearEmpresaAtual();
    EXPECT_FALSE(state.empresaAtual().has_value());
    EXPECT_EQ(spy.count(), 2);
}

TEST(MensagensTest, ValidacaoAndOperacaoFormatting) {
    EXPECT_EQ(Mensagens::campoObrigatorio("Razão Social"),
              "O campo Razão Social é de preenchimento obrigatório.");
    EXPECT_EQ(Mensagens::Validacao::selecaoObrigatoria("Unidade de Medida"),
              "A seleção de Unidade de Medida é obrigatória.");
    EXPECT_EQ(Mensagens::preenchaCamposObrigatorios("CPF/CNPJ e Razão Social"),
              "Preencha os campos obrigatórios (CPF/CNPJ e Razão Social).");
    EXPECT_EQ(Mensagens::campoInvalido("CNPJ"),
              "O campo CNPJ informado é inválido.");
    EXPECT_EQ(Mensagens::tamanhoMinimo("Senha", 6),
              "O campo Senha deve ter no mínimo 6 caracteres.");
    EXPECT_EQ(Mensagens::tamanhoMaximo("Observação", 120),
              "O campo Observação deve ter no máximo 120 caracteres.");
    EXPECT_EQ(Mensagens::tamanhoExato("CEP", 8),
              "O campo CEP deve ter exatamente 8 caracteres.");
    EXPECT_EQ(Mensagens::intervaloCaracteres("Código", 3, 10),
              "O campo Código deve ter entre 3 e 10 caracteres.");
    EXPECT_EQ(Mensagens::valorMaiorQueZero("Quantidade"),
              "O campo Quantidade deve possuir um valor maior que zero.");
    EXPECT_EQ(Mensagens::registroJaExiste("Código de Barras"),
              "Já existe um registro com este(a) Código de Barras.");

    EXPECT_EQ(Mensagens::salvoSucesso("Produto"),
              "Produto salvo(a) com sucesso!");
    EXPECT_EQ(Mensagens::erroSalvar("Empresa", "Violação de chave única"),
              "Ocorreu um erro ao salvar Empresa:\nViolação de chave única");
    EXPECT_EQ(Mensagens::confirmarExclusao("Participante"),
              "Deseja realmente excluir este(a) Participante?");
    EXPECT_EQ(Mensagens::excluidoSucesso("Participante"),
              "Participante excluído(a) com sucesso!");
    EXPECT_EQ(Mensagens::nenhumRegistroEncontrado(),
              "Nenhum registro encontrado.");
}

