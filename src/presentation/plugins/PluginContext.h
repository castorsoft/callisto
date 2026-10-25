#pragma once

#include "../../plugins/api/IPluginContext.h"
#include "../../infrastructure/database/repositories/EmpresaRepository.h"
#include "../../infrastructure/database/repositories/DocumentoFiscalRepository.h"
#include "../../infrastructure/database/repositories/PessoaRepository.h"
#include "../../infrastructure/database/repositories/ProdutoRepository.h"
#include "../../infrastructure/database/repositories/ProdutoFornecedorRepository.h"
#include "../../infrastructure/database/repositories/UnidadeMedidaRepository.h"
#include <functional>

class PluginContext : public IPluginContext {
public:
    using TabOpener = std::function<void(QWidget*, const QString&)>;

    PluginContext();
    ~PluginContext() override = default;

    void setTabOpener(TabOpener opener);

    std::optional<Empresa> empresaAtual() const override;

    IEmpresaRepository& empresaRepository() const override;
    IDocumentoFiscalRepository& documentoFiscalRepository() const override;
    IPessoaRepository& pessoaRepository() const override;
    IProdutoRepository& produtoRepository() const override;
    IProdutoFornecedorRepository& produtoFornecedorRepository() const override;
    IUnidadeMedidaRepository& unidadeMedidaRepository() const override;

    void openTab(QWidget *widget, const QString &title) override;

    // Métodos Factory de Componentes Visuais (Inputs Padrão do Sistema)
    InputText* createInputText(const QString &label, const QString &placeholder = "", QWidget *parent = nullptr) const override;
    InputMasked* createInputMasked(const QString &label, const QString &mask = "", QWidget *parent = nullptr) const override;
    InputComboBox* createInputComboBox(const QString &label, QWidget *parent = nullptr) const override;
    InputCheckBox* createInputCheckBox(const QString &label, const QString &text = "", QWidget *parent = nullptr) const override;

    // Métodos Factory de Containers e Layouts
    QGroupBox* createGroupBox(const QString &title, QWidget *parent = nullptr) const override;
    QHBoxLayout* createHBoxLayout(QWidget *parent = nullptr) const override;
    QVBoxLayout* createVBoxLayout(QWidget *parent = nullptr) const override;
    QGridLayout* createGridLayout(QWidget *parent = nullptr) const override;

    // Métodos Factory de Espaçadores
    QSpacerItem* createHorizontalSpacer(int width = 20, int height = 20) const override;
    QSpacerItem* createVerticalSpacer(int width = 20, int height = 20) const override;
    QWidget* createHorizontalSpacing(int width = 10, QWidget *parent = nullptr) const override;
    QWidget* createVerticalSpacing(int height = 10, QWidget *parent = nullptr) const override;

private:
    mutable EmpresaRepository m_empresaRepo;
    mutable DocumentoFiscalRepository m_docFiscalRepo;
    mutable PessoaRepository m_pessoaRepo;
    mutable ProdutoRepository m_produtoRepo;
    mutable ProdutoFornecedorRepository m_produtoFornecedorRepo;
    mutable UnidadeMedidaRepository m_unidadeMedidaRepo;

    TabOpener m_tabOpener;
};
