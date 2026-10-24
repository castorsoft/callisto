#pragma once

#include <QString>

/**
 * @file Mensagens.h
 * @brief Centralização de constantes de texto e templates de mensagens com parâmetros
 *        para validações de formulários, notificações e diálogos do sistema.
 * 
 * Exemplos de uso:
 * 
 *   // 1. Chamada direta de função utilitária:
 *   QString msg = Mensagens::campoObrigatorio("Descrição");
 *   // Retorna: "O campo Descrição é de preenchimento obrigatório."
 * 
 *   // 2. Com categoria explícita:
 *   QString msg = Mensagens::Validacao::campoObrigatorio("Descrição");
 * 
 *   // 3. Acesso à constante de formato bruto (com .arg()):
 *   QString msg = QString(Mensagens::Formatos::CAMPO_OBRIGATORIO).arg("Descrição");
 */

namespace Mensagens {

// ============================================================================
// CONSTANTES DE FORMATO BRUTO (Templates para QString::arg)
// ============================================================================
namespace Formatos {
    // Validações de preenchimento
    inline constexpr const char* CAMPO_OBRIGATORIO            = "O campo %1 é de preenchimento obrigatório.";
    inline constexpr const char* SELECAO_OBRIGATORIA          = "A seleção de %1 é obrigatória.";
    inline constexpr const char* PREENCHA_CAMPOS_OBRIGATORIOS = "Preencha os campos obrigatórios (%1).";
    inline constexpr const char* CAMPO_INVALIDO               = "O campo %1 informado é inválido.";

    // Validações de tamanho e limites
    inline constexpr const char* TAMANHO_MINIMO               = "O campo %1 deve ter no mínimo %2 caracteres.";
    inline constexpr const char* TAMANHO_MAXIMO               = "O campo %1 deve ter no máximo %2 caracteres.";
    inline constexpr const char* TAMANHO_EXATO                = "O campo %1 deve ter exatamente %2 caracteres.";
    inline constexpr const char* INTERVALO_CARACTERES         = "O campo %1 deve ter entre %2 e %3 caracteres.";
    inline constexpr const char* VALOR_MAIOR_QUE_ZERO         = "O campo %1 deve possuir um valor maior que zero.";
    inline constexpr const char* VALOR_MINIMO                 = "O campo %1 deve ser maior ou igual a %2.";
    inline constexpr const char* VALOR_MAXIMO                 = "O campo %1 deve ser menor ou igual a %2.";

    // Validações específicas comuns
    inline constexpr const char* REGISTRO_JA_EXISTE           = "Já existe um registro com este(a) %1.";
    inline constexpr const char* NENHUM_REGISTRO_ENCONTRADO    = "Nenhum registro encontrado.";

    // Operações de CRUD e sistema
    inline constexpr const char* REGISTRO_SALVO_SUCESSO       = "%1 salvo(a) com sucesso!";
    inline constexpr const char* ERRO_SALVAR_REGISTRO         = "Ocorreu um erro ao salvar %1:\n%2";
    inline constexpr const char* CONFIRMAR_EXCLUSAO           = "Deseja realmente excluir este(a) %1?";
    inline constexpr const char* REGISTRO_EXCLUIDO_SUCESSO     = "%1 excluído(a) com sucesso!";
} // namespace Formatos

// ============================================================================
// FUNÇÕES UTILITÁRIAS DE VALIDAÇÃO (Substituição parametrizada pronta)
// ============================================================================
namespace Validacao {

    /**
     * @brief Retorna mensagem informando que o campo é obrigatório.
     * Ex: "O campo Descrição é de preenchimento obrigatório."
     */
    inline QString campoObrigatorio(const QString &nomeCampo) {
        return QString(Formatos::CAMPO_OBRIGATORIO).arg(nomeCampo);
    }

    /**
     * @brief Retorna mensagem informando que a seleção de uma opção é obrigatória.
     * Ex: "A seleção de Unidade de Medida é obrigatória."
     */
    inline QString selecaoObrigatoria(const QString &nomeCampo) {
        return QString(Formatos::SELECAO_OBRIGATORIA).arg(nomeCampo);
    }

    /**
     * @brief Retorna mensagem para múltiplos campos obrigatórios não preenchidos.
     * Ex: "Preencha os campos obrigatórios (CPF/CNPJ e Razão Social)."
     */
    inline QString preenchaCamposObrigatorios(const QString &nomesCampos) {
        return QString(Formatos::PREENCHA_CAMPOS_OBRIGATORIOS).arg(nomesCampos);
    }

    /**
     * @brief Retorna mensagem informando que o valor digitado no campo é inválido.
     * Ex: "O campo CPF/CNPJ informado é inválido."
     */
    inline QString campoInvalido(const QString &nomeCampo) {
        return QString(Formatos::CAMPO_INVALIDO).arg(nomeCampo);
    }

    /**
     * @brief Retorna mensagem de quantidade mínima de caracteres.
     * Ex: "O campo Senha deve ter no mínimo 6 caracteres."
     */
    inline QString tamanhoMinimo(const QString &nomeCampo, int min) {
        return QString(Formatos::TAMANHO_MINIMO).arg(nomeCampo).arg(min);
    }

    /**
     * @brief Retorna mensagem de quantidade máxima de caracteres.
     * Ex: "O campo Observação deve ter no máximo 255 caracteres."
     */
    inline QString tamanhoMaximo(const QString &nomeCampo, int max) {
        return QString(Formatos::TAMANHO_MAXIMO).arg(nomeCampo).arg(max);
    }

    /**
     * @brief Retorna mensagem de tamanho fixo exigido.
     * Ex: "O campo CEP deve ter exatamente 8 caracteres."
     */
    inline QString tamanhoExato(const QString &nomeCampo, int tamanho) {
        return QString(Formatos::TAMANHO_EXATO).arg(nomeCampo).arg(tamanho);
    }

    /**
     * @brief Retorna mensagem de faixa de caracteres permitida.
     * Ex: "O campo Código deve ter entre 3 e 10 caracteres."
     */
    inline QString intervaloCaracteres(const QString &nomeCampo, int min, int max) {
        return QString(Formatos::INTERVALO_CARACTERES).arg(nomeCampo).arg(min).arg(max);
    }

    /**
     * @brief Retorna mensagem informando que o campo numérico deve ser maior que zero.
     * Ex: "O campo Quantidade deve possuir um valor maior que zero."
     */
    inline QString valorMaiorQueZero(const QString &nomeCampo) {
        return QString(Formatos::VALOR_MAIOR_QUE_ZERO).arg(nomeCampo);
    }

    /**
     * @brief Retorna mensagem de duplicidade de registro.
     * Ex: "Já existe um registro com este(a) Código de Barras."
     */
    inline QString registroJaExiste(const QString &nomeCampo) {
        return QString(Formatos::REGISTRO_JA_EXISTE).arg(nomeCampo);
    }

} // namespace Validacao

// ============================================================================
// FUNÇÕES UTILITÁRIAS DE OPERAÇÕES DO SISTEMA (CRUD / AVISOS)
// ============================================================================
namespace Operacao {

    /**
     * @brief Retorna mensagem de sucesso ao salvar registro.
     * Ex: "Produto salvo(a) com sucesso!"
     */
    inline QString salvoSucesso(const QString &nomeEntidade) {
        return QString(Formatos::REGISTRO_SALVO_SUCESSO).arg(nomeEntidade);
    }

    /**
     * @brief Retorna mensagem formatada com o erro retornado ao salvar.
     */
    inline QString erroSalvar(const QString &nomeEntidade, const QString &erroDetalhe) {
        return QString(Formatos::ERRO_SALVAR_REGISTRO).arg(nomeEntidade, erroDetalhe);
    }

    /**
     * @brief Retorna pergunta de confirmação de exclusão.
     * Ex: "Deseja realmente excluir este(a) Produto?"
     */
    inline QString confirmarExclusao(const QString &nomeEntidade) {
        return QString(Formatos::CONFIRMAR_EXCLUSAO).arg(nomeEntidade);
    }

    /**
     * @brief Retorna mensagem de sucesso após exclusão.
     * Ex: "Produto excluído(a) com sucesso!"
     */
    inline QString excluidoSucesso(const QString &nomeEntidade) {
        return QString(Formatos::REGISTRO_EXCLUIDO_SUCESSO).arg(nomeEntidade);
    }

    inline QString nenhumRegistroEncontrado() {
        return QString(Formatos::NENHUM_REGISTRO_ENCONTRADO);
    }

} // namespace Operacao

// ============================================================================
// FACILITADORES NO ESCOPO RAIZ DE Mensagens::
// Permite chamar diretamente Mensagens::campoObrigatorio(...) sem Validacao::
// ============================================================================
using namespace Validacao;
using namespace Operacao;

} // namespace Mensagens

