#include "CastorWidgetsPlugin.h"
#include "InputTextPlugin.h"
#include "InputMaskedPlugin.h"
#include "InputComboBoxPlugin.h"
#include "InputCheckBoxPlugin.h"
#include "InputRadioGroupPlugin.h"
#include "InputLookupPlugin.h"

CastorWidgetsPlugin::CastorWidgetsPlugin(QObject *parent)
    : QObject(parent)
{
    m_widgets.append(new InputTextPlugin(this));
    m_widgets.append(new InputMaskedPlugin(this));
    m_widgets.append(new InputComboBoxPlugin(this));
    m_widgets.append(new InputCheckBoxPlugin(this));
    m_widgets.append(new InputRadioGroupPlugin(this));
    m_widgets.append(new InputLookupPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> CastorWidgetsPlugin::customWidgets() const
{
    return m_widgets;
}

