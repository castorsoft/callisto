#include "InputComboBoxPlugin.h"
#include "presentation/components/inputs/InputComboBox.h"

InputComboBoxPlugin::InputComboBoxPlugin(QObject *parent)
    : QObject(parent)
{
}

void InputComboBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    m_initialized = true;
}

bool InputComboBoxPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *InputComboBoxPlugin::createWidget(QWidget *parent)
{
    return new InputComboBox(parent);
}

QString InputComboBoxPlugin::name() const
{
    return QStringLiteral("InputComboBox");
}

QString InputComboBoxPlugin::group() const
{
    return QStringLiteral("Castor Inputs");
}

QIcon InputComboBoxPlugin::icon() const
{
    return QIcon();
}

QString InputComboBoxPlugin::toolTip() const
{
    return QStringLiteral("Caixa de seleção suspensa com rótulo e proteção contra scroll acidental");
}

QString InputComboBoxPlugin::whatsThis() const
{
    return QStringLiteral("Widget de seleção com rótulo superior e filtragem de eventos de rolagem");
}

bool InputComboBoxPlugin::isContainer() const
{
    return false;
}

QString InputComboBoxPlugin::domXml() const
{
    return QStringLiteral(R"(<ui language="c++">
  <widget class="InputComboBox" name="inputComboBox">
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>200</width>
        <height>46</height>
      </rect>
    </property>
    <property name="labelText">
      <string>Opção:</string>
    </property>
  </widget>
</ui>)");
}

QString InputComboBoxPlugin::includeFile() const
{
    return QStringLiteral("presentation/components/inputs/InputComboBox.h");
}

