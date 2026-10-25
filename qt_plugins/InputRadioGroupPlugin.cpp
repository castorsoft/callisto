#include "InputRadioGroupPlugin.h"
#include "presentation/components/inputs/InputRadioGroup.h"

InputRadioGroupPlugin::InputRadioGroupPlugin(QObject *parent)
    : QObject(parent)
{
}

void InputRadioGroupPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    m_initialized = true;
}

bool InputRadioGroupPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *InputRadioGroupPlugin::createWidget(QWidget *parent)
{
    auto *widget = new InputRadioGroup(parent);
    widget->setLabelText(QStringLiteral("Opções:"));
    widget->addOption(QStringLiteral("Opção 1"));
    widget->addOption(QStringLiteral("Opção 2"));
    return widget;
}

QString InputRadioGroupPlugin::name() const
{
    return QStringLiteral("InputRadioGroup");
}

QString InputRadioGroupPlugin::group() const
{
    return QStringLiteral("Castor Inputs");
}

QIcon InputRadioGroupPlugin::icon() const
{
    return QIcon();
}

QString InputRadioGroupPlugin::toolTip() const
{
    return QStringLiteral("Grupo de botões de opção exclusiva com alinhamento configurável");
}

QString InputRadioGroupPlugin::whatsThis() const
{
    return QStringLiteral("Widget de grupo de botões de rádio com layout horizontal ou vertical");
}

bool InputRadioGroupPlugin::isContainer() const
{
    return false;
}

QString InputRadioGroupPlugin::domXml() const
{
    return QStringLiteral(R"(<ui language="c++">
  <widget class="InputRadioGroup" name="inputRadioGroup">
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>200</width>
        <height>50</height>
      </rect>
    </property>
    <property name="labelText">
      <string>Opções:</string>
    </property>
  </widget>
</ui>)");
}

QString InputRadioGroupPlugin::includeFile() const
{
    return QStringLiteral("presentation/components/inputs/InputRadioGroup.h");
}

