#include "InputButtonPlugin.h"
#include "presentation/components/inputs/InputButton.h"

InputButtonPlugin::InputButtonPlugin(QObject *parent)
    : QObject(parent)
{
}

void InputButtonPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    m_initialized = true;
}

bool InputButtonPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *InputButtonPlugin::createWidget(QWidget *parent)
{
    return new InputButton(parent);
}

QString InputButtonPlugin::name() const
{
    return QStringLiteral("InputButton");
}

QString InputButtonPlugin::group() const
{
    return QStringLiteral("Castor Inputs");
}

QIcon InputButtonPlugin::icon() const
{
    return QIcon();
}

QString InputButtonPlugin::toolTip() const
{
    return QStringLiteral("Botão customizado com esquemas de cores adaptáveis ao tema do Qt");
}

QString InputButtonPlugin::whatsThis() const
{
    return QStringLiteral("Widget de botão customizado derivado de QPushButton com suporte a ColorScheme (Default, Primary, Secondary, Success, Warning, Danger)");
}

bool InputButtonPlugin::isContainer() const
{
    return false;
}

QString InputButtonPlugin::domXml() const
{
    return QStringLiteral(R"(<ui language="c++">
  <widget class="InputButton" name="inputButton">
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>110</width>
        <height>32</height>
      </rect>
    </property>
    <property name="text">
      <string>Botão</string>
    </property>
  </widget>
</ui>)");
}

QString InputButtonPlugin::includeFile() const
{
    return QStringLiteral("presentation/components/inputs/InputButton.h");
}

