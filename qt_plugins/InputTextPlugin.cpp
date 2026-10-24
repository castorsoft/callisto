#include "InputTextPlugin.h"
#include "presentation/components/inputs/InputText.h"

InputTextPlugin::InputTextPlugin(QObject *parent)
    : QObject(parent)
{
}

void InputTextPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    m_initialized = true;
}

bool InputTextPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *InputTextPlugin::createWidget(QWidget *parent)
{
    return new InputText(parent);
}

QString InputTextPlugin::name() const
{
    return QStringLiteral("InputText");
}

QString InputTextPlugin::group() const
{
    return QStringLiteral("Castor Inputs");
}

QIcon InputTextPlugin::icon() const
{
    return QIcon();
}

QString InputTextPlugin::toolTip() const
{
    return QStringLiteral("Campo de entrada de texto com rótulo e tratamento de erro");
}

QString InputTextPlugin::whatsThis() const
{
    return QStringLiteral("Widget de texto customizado com rótulo superior e indicador de validação/erro");
}

bool InputTextPlugin::isContainer() const
{
    return false;
}

QString InputTextPlugin::domXml() const
{
    return QStringLiteral(R"(<ui language="c++">
  <widget class="InputText" name="inputText">
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>200</width>
        <height>46</height>
      </rect>
    </property>
    <property name="labelText">
      <string>Texto:</string>
    </property>
  </widget>
</ui>)");
}

QString InputTextPlugin::includeFile() const
{
    return QStringLiteral("presentation/components/inputs/InputText.h");
}

