#include "InputText.h"

InputText::InputText(QWidget *parent) : InputBase(parent) {
    m_lineEdit = new QLineEdit(this);

    // Add specific style to the line edit if needed
    // m_lineEdit->setStyleSheet("padding: 4px; border: 1px solid #ccc; border-radius: 4px;");

    setInputWidget(m_lineEdit);

    connect(m_lineEdit, &QLineEdit::textChanged, this, &InputText::textChanged);
}

QString InputText::text() const {
    return m_lineEdit->text();
}

void InputText::setText(const QString &text) {
    m_lineEdit->setText(text);
}

QString InputText::placeholderText() const {
    return m_lineEdit->placeholderText();
}

void InputText::setPlaceholderText(const QString &text) {
    m_lineEdit->setPlaceholderText(text);
}
