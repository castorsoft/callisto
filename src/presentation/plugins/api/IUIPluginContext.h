#pragma once

#include "core/plugins/IPluginContext.h"
#include <QString>

class QWidget;
class QGroupBox;
class QScrollArea;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QSpacerItem;

class InputText;
class InputMasked;
class InputComboBox;
class InputCheckBox;
class InputLookup;
class InputButton;

class IUIPluginContext : public virtual IPluginContext {
public:
    ~IUIPluginContext() override = default;

    // Serviços de Interface Gráfica (UI)
    virtual void openTab(QWidget *widget, const QString &title) = 0;

    // Métodos Factory de Componentes Visuais (Inputs Padrão do Sistema)
    virtual InputText* createInputText(const QString &label, const QString &placeholder = "", QWidget *parent = nullptr) const = 0;
    virtual InputMasked* createInputMasked(const QString &label, const QString &mask = "", QWidget *parent = nullptr) const = 0;
    virtual InputComboBox* createInputComboBox(const QString &label, QWidget *parent = nullptr) const = 0;
    virtual InputCheckBox* createInputCheckBox(const QString &label, const QString &text = "", QWidget *parent = nullptr) const = 0;
    virtual InputLookup* createInputLookup(const QString &label, const QString &placeholder = "", QWidget *parent = nullptr) const = 0;
    virtual InputButton* createInputButton(const QString &text, QWidget *parent = nullptr) const = 0;

    // Métodos Factory de Containers e Layouts
    virtual QGroupBox* createGroupBox(const QString &title, QWidget *parent = nullptr) const = 0;
    virtual QScrollArea* createScrollArea(QWidget *parent = nullptr) const = 0;
    virtual QHBoxLayout* createHBoxLayout(QWidget *parent = nullptr) const = 0;
    virtual QVBoxLayout* createVBoxLayout(QWidget *parent = nullptr) const = 0;
    virtual QGridLayout* createGridLayout(QWidget *parent = nullptr) const = 0;

    // Métodos Factory de Espaçadores
    virtual QSpacerItem* createHorizontalSpacer(int width = 20, int height = 20) const = 0;
    virtual QSpacerItem* createVerticalSpacer(int width = 20, int height = 20) const = 0;
    virtual QWidget* createHorizontalSpacing(int width = 10, QWidget *parent = nullptr) const = 0;
    virtual QWidget* createVerticalSpacing(int height = 10, QWidget *parent = nullptr) const = 0;
};

