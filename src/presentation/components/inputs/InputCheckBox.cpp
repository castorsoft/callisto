#include "InputCheckBox.h"

InputCheckBox::InputCheckBox(QWidget *parent) : InputBase(parent) {
    m_checkBox = new QCheckBox(this);
    setInputWidget(m_checkBox);

    connect(m_checkBox, &QCheckBox::toggled, this, &InputCheckBox::toggled);
}

bool InputCheckBox::isChecked() const {
    return m_checkBox->isChecked();
}

void InputCheckBox::setChecked(bool checked) {
    m_checkBox->setChecked(checked);
}

QString InputCheckBox::text() const {
    return m_checkBox->text();
}

void InputCheckBox::setText(const QString &text) {
    m_checkBox->setText(text);
}
