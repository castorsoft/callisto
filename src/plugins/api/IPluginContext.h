#pragma once

#include <QString>
#include <QDate>
#include <memory>
#include <optional>
#include "../../core/entities/Empresa.h"
#include "../../core/interfaces/IEmpresaRepository.h"
#include "../../core/interfaces/IDocumentoFiscalRepository.h"
#include "../../core/interfaces/IPessoaRepository.h"
#include "../../core/interfaces/IProdutoRepository.h"
#include "../../core/interfaces/IProdutoFornecedorRepository.h"
#include "../../core/interfaces/IUnidadeMedidaRepository.h"

class QWidget;
class QGroupBox;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QSpacerItem;

class InputText;
class InputMasked;
class InputComboBox;
class InputCheckBox;

class IPluginContext {
public:
    virtual ~IPluginContext() = default;

    // Estado da Aplicação
    virtual std::optional<Empresa> empresaAtual() const = 0;

    // Acesso aos Repositórios
    virtual IEmpresaRepository& empresaRepository() const = 0;
    virtual IDocumentoFiscalRepository& documentoFiscalRepository() const = 0;
    virtual IPessoaRepository& pessoaRepository() const = 0;
    virtual IProdutoRepository& produtoRepository() const = 0;
    virtual IProdutoFornecedorRepository& produtoFornecedorRepository() const = 0;
    virtual IUnidadeMedidaRepository& unidadeMedidaRepository() const = 0;

    // Serviços de Interface Gráfica (UI)
    virtual void openTab(QWidget *widget, const QString &title) = 0;

    // Métodos Factory de Componentes Visuais (Inputs Padrão do Sistema)
    virtual InputText* createInputText(const QString &label, const QString &placeholder = "", QWidget *parent = nullptr) const = 0;
    virtual InputMasked* createInputMasked(const QString &label, const QString &mask = "", QWidget *parent = nullptr) const = 0;
    virtual InputComboBox* createInputComboBox(const QString &label, QWidget *parent = nullptr) const = 0;
    virtual InputCheckBox* createInputCheckBox(const QString &label, const QString &text = "", QWidget *parent = nullptr) const = 0;

    // Métodos Factory de Containers e Layouts
    virtual QGroupBox* createGroupBox(const QString &title, QWidget *parent = nullptr) const = 0;
    virtual QHBoxLayout* createHBoxLayout(QWidget *parent = nullptr) const = 0;
    virtual QVBoxLayout* createVBoxLayout(QWidget *parent = nullptr) const = 0;
    virtual QGridLayout* createGridLayout(QWidget *parent = nullptr) const = 0;

    // Métodos Factory de Espaçadores
    virtual QSpacerItem* createHorizontalSpacer(int width = 20, int height = 20) const = 0;
    virtual QSpacerItem* createVerticalSpacer(int width = 20, int height = 20) const = 0;
    virtual QWidget* createHorizontalSpacing(int width = 10, QWidget *parent = nullptr) const = 0;
    virtual QWidget* createVerticalSpacing(int height = 10, QWidget *parent = nullptr) const = 0;
};
