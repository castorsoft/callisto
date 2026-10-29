#include "PluginContext.h"
#include "../../core/GlobalState.h"
#include "../components/inputs/InputText.h"
#include "../components/inputs/InputMasked.h"
#include "../components/inputs/InputComboBox.h"
#include "../components/inputs/InputCheckBox.h"
#include <QWidget>
#include <QGroupBox>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>

PluginContext::PluginContext() = default;

void PluginContext::setTabOpener(TabOpener opener)
{
    m_tabOpener = std::move(opener);
}

std::optional<Empresa> PluginContext::empresaAtual() const
{
    return GlobalState::instance().empresaAtual();
}

IEmpresaRepository& PluginContext::empresaRepository() const
{
    return m_empresaRepo;
}

IDocumentoFiscalRepository& PluginContext::documentoFiscalRepository() const
{
    return m_docFiscalRepo;
}

IPessoaRepository& PluginContext::pessoaRepository() const
{
    return m_pessoaRepo;
}

IProdutoRepository& PluginContext::produtoRepository() const
{
    return m_produtoRepo;
}

IProdutoFornecedorRepository& PluginContext::produtoFornecedorRepository() const
{
    return m_produtoFornecedorRepo;
}

IUnidadeMedidaRepository& PluginContext::unidadeMedidaRepository() const
{
    return m_unidadeMedidaRepo;
}

void PluginContext::openTab(QWidget *widget, const QString &title)
{
    if (m_tabOpener && widget) {
        m_tabOpener(widget, title);
    }
}

InputText* PluginContext::createInputText(const QString &label, const QString &placeholder, QWidget *parent) const
{
    auto *input = new InputText(parent);
    input->setLabelText(label);
    if (!placeholder.isEmpty()) {
        input->setPlaceholderText(placeholder);
    }
    return input;
}

InputMasked* PluginContext::createInputMasked(const QString &label, const QString &mask, QWidget *parent) const
{
    auto *input = new InputMasked(parent);
    input->setLabelText(label);
    if (!mask.isEmpty()) {
        input->setMask(mask);
    }
    return input;
}

InputComboBox* PluginContext::createInputComboBox(const QString &label, QWidget *parent) const
{
    auto *input = new InputComboBox(parent);
    input->setLabelText(label);
    return input;
}

InputCheckBox* PluginContext::createInputCheckBox(const QString &label, const QString &text, QWidget *parent) const
{
    auto *input = new InputCheckBox(parent);
    input->setLabelText(label);
    if (!text.isEmpty()) {
        input->setText(text);
    }
    return input;
}

QGroupBox* PluginContext::createGroupBox(const QString &title, QWidget *parent) const
{
    return new QGroupBox(title, parent);
}

QScrollArea* PluginContext::createScrollArea(QWidget *parent) const
{
    auto *scrollArea = new QScrollArea(parent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    return scrollArea;
}

QHBoxLayout* PluginContext::createHBoxLayout(QWidget *parent) const
{
    auto *layout = new QHBoxLayout(parent);
    layout->setSpacing(8);
    layout->setContentsMargins(0, 0, 0, 0);
    return layout;
}

QVBoxLayout* PluginContext::createVBoxLayout(QWidget *parent) const
{
    auto *layout = new QVBoxLayout(parent);
    layout->setSpacing(8);
    layout->setContentsMargins(0, 0, 0, 0);
    return layout;
}

QGridLayout* PluginContext::createGridLayout(QWidget *parent) const
{
    auto *layout = new QGridLayout(parent);
    layout->setSpacing(8);
    layout->setContentsMargins(0, 0, 0, 0);
    return layout;
}

QSpacerItem* PluginContext::createHorizontalSpacer(int width, int height) const
{
    return new QSpacerItem(width, height, QSizePolicy::Expanding, QSizePolicy::Minimum);
}

QSpacerItem* PluginContext::createVerticalSpacer(int width, int height) const
{
    return new QSpacerItem(width, height, QSizePolicy::Minimum, QSizePolicy::Expanding);
}

QWidget* PluginContext::createHorizontalSpacing(int width, QWidget *parent) const
{
    auto *spacer = new QWidget(parent);
    spacer->setFixedWidth(width);
    return spacer;
}

QWidget* PluginContext::createVerticalSpacing(int height, QWidget *parent) const
{
    auto *spacer = new QWidget(parent);
    spacer->setFixedHeight(height);
    return spacer;
}
