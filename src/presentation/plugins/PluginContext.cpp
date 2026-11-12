#include "PluginContext.h"
#include "../../core/GlobalState.h"
#include "../../core/RepositoryProvider.h"
#include "../components/inputs/InputText.h"
#include "../components/inputs/InputMasked.h"
#include "../components/inputs/InputComboBox.h"
#include "../components/inputs/InputCheckBox.h"
#include "../components/inputs/InputLookup.h"
#include "../components/inputs/InputButton.h"
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
    return RepositoryProvider::empresaRepository();
}

IDocumentoFiscalRepository& PluginContext::documentoFiscalRepository() const
{
    return RepositoryProvider::documentoFiscalRepository();
}

IPessoaRepository& PluginContext::pessoaRepository() const
{
    return RepositoryProvider::pessoaRepository();
}

IProdutoRepository& PluginContext::produtoRepository() const
{
    return RepositoryProvider::produtoRepository();
}

IProdutoFornecedorRepository& PluginContext::produtoFornecedorRepository() const
{
    return RepositoryProvider::produtoFornecedorRepository();
}

IUnidadeMedidaRepository& PluginContext::unidadeMedidaRepository() const
{
    return RepositoryProvider::unidadeMedidaRepository();
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

InputLookup* PluginContext::createInputLookup(const QString &label, const QString &placeholder, QWidget *parent) const
{
    auto *input = new InputLookup(parent);
    input->setLabelText(label);
    if (!placeholder.isEmpty()) {
        input->setPlaceholderText(placeholder);
    }
    return input;
}

InputButton* PluginContext::createInputButton(const QString &text, QWidget *parent) const
{
    return new InputButton(text, parent);
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
