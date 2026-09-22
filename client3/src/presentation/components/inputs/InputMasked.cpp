#include "InputMasked.h"
#include <QRegularExpression>

InputMasked::InputMasked(QWidget *parent) : InputBase(parent) {
    m_lineEdit = new QLineEdit(this);

    setInputWidget(m_lineEdit);

    connect(m_lineEdit, &QLineEdit::textChanged, this, [this](const QString &) {
        emit textChanged(text());
    });
}

QString InputMasked::text() const {
    // Return only the digits, removing the mask characters
    QString raw = m_lineEdit->text();
    raw.remove(QRegularExpression("[^0-9]"));
    return raw;
}

QString InputMasked::mask() const
{
    return m_lineEdit->inputMask();
}

void InputMasked::setText(const QString &text) {
    // Setting raw numbers will be automatically masked by the QLineEdit
    m_lineEdit->setText(text);
}

void InputMasked::setMask(const QString &mask) {
    m_lineEdit->setInputMask(mask);
}
