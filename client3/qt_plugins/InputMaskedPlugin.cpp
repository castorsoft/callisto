#include "InputMaskedPlugin.h"
#include "presentation/components/inputs/InputMasked.h"

InputMaskedPlugin::InputMaskedPlugin(QObject *parent)
    : QObject(parent)
{
}

void InputMaskedPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    m_initialized = true;
}

bool InputMaskedPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *InputMaskedPlugin::createWidget(QWidget *parent)
{
    return new InputMasked(parent);
}

QString InputMaskedPlugin::name() const
{
    return QStringLiteral("InputMasked");
}

QString InputMaskedPlugin::group() const
{
    return QStringLiteral("Castor Inputs");
}

QIcon InputMaskedPlugin::icon() const
{
    return QIcon();
}

QString InputMaskedPlugin::toolTip() const
{
    return QStringLiteral("Campo de entrada com máscara formatada e validação");
}

QString InputMaskedPlugin::whatsThis() const
{
    return QStringLiteral("Widget de texto customizado com suporte a máscara de entrada (CPF, CNPJ, CEP, etc.)");
}

bool InputMaskedPlugin::isContainer() const
{
    return false;
}

QString InputMaskedPlugin::domXml() const
{
    return QStringLiteral(R"(<ui language="c++">
  <widget class="InputMasked" name="inputMasked">
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>200</width>
        <height>46</height>
      </rect>
    </property>
    <property name="labelText">
      <string>Campo Mascarado:</string>
    </property>
  </widget>
</ui>)");
}

QString InputMaskedPlugin::includeFile() const
{
    return QStringLiteral("presentation/components/inputs/InputMasked.h");
}

