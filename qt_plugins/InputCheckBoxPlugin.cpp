#include "InputCheckBoxPlugin.h"
#include "presentation/components/inputs/InputCheckBox.h"

InputCheckBoxPlugin::InputCheckBoxPlugin(QObject *parent)
    : QObject(parent)
{
}

void InputCheckBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    m_initialized = true;
}

bool InputCheckBoxPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *InputCheckBoxPlugin::createWidget(QWidget *parent)
{
    auto *widget = new InputCheckBox(parent);
    widget->setText(QStringLiteral("Opção"));
    return widget;
}

QString InputCheckBoxPlugin::name() const
{
    return QStringLiteral("InputCheckBox");
}

QString InputCheckBoxPlugin::group() const
{
    return QStringLiteral("Castor Inputs");
}

QIcon InputCheckBoxPlugin::icon() const
{
    return QIcon();
}

QString InputCheckBoxPlugin::toolTip() const
{
    return QStringLiteral("Caixa de marcação customizada com texto e rótulo");
}

QString InputCheckBoxPlugin::whatsThis() const
{
    return QStringLiteral("Widget de caixa de seleção booleana customizado da Castor");
}

bool InputCheckBoxPlugin::isContainer() const
{
    return false;
}

QString InputCheckBoxPlugin::domXml() const
{
    return QStringLiteral(R"(<ui language="c++">
  <widget class="InputCheckBox" name="inputCheckBox">
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>150</width>
        <height>30</height>
      </rect>
    </property>
    <property name="text">
      <string>Opção</string>
    </property>
  </widget>
</ui>)");
}

QString InputCheckBoxPlugin::includeFile() const
{
    return QStringLiteral("presentation/components/inputs/InputCheckBox.h");
}

