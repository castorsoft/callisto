#include "InputLookupPlugin.h"
#include "presentation/components/inputs/InputLookup.h"

InputLookupPlugin::InputLookupPlugin(QObject *parent)
    : QObject(parent)
{
}

void InputLookupPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    m_initialized = true;
}

bool InputLookupPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *InputLookupPlugin::createWidget(QWidget *parent)
{
    auto *widget = new InputLookup(parent);
    widget->setLabelText(QStringLiteral("NCM:"));
    widget->setPlaceholderText(QStringLiteral("Digite o código ou descrição para buscar..."));
    return widget;
}

QString InputLookupPlugin::name() const
{
    return QStringLiteral("InputLookup");
}

QString InputLookupPlugin::group() const
{
    return QStringLiteral("Castor Inputs");
}

QIcon InputLookupPlugin::icon() const
{
    return QIcon();
}

QString InputLookupPlugin::toolTip() const
{
    return QStringLiteral("Campo de busca com sugestões dinâmicas em lista suspensa (ideal para NCMs e grandes listas)");
}

QString InputLookupPlugin::whatsThis() const
{
    return QStringLiteral("Componente de busca com autocompletar e debounce, exibindo opções sob demanda em vez de carregar listas pesadas");
}

bool InputLookupPlugin::isContainer() const
{
    return false;
}

QString InputLookupPlugin::domXml() const
{
    return QStringLiteral(R"(<ui language="c++">
  <widget class="InputLookup" name="inputLookup">
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>250</width>
        <height>46</height>
      </rect>
    </property>
    <property name="labelText">
      <string>Buscar:</string>
    </property>
    <property name="placeholderText">
      <string>Digite para buscar...</string>
    </property>
  </widget>
</ui>)");
}

QString InputLookupPlugin::includeFile() const
{
    return QStringLiteral("presentation/components/inputs/InputLookup.h");
}

