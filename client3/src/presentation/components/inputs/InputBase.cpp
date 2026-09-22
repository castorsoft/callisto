#include "InputBase.h"

InputBase::InputBase(QWidget *parent) : QWidget(parent) {
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(4);

    m_labelTitle = new QLabel(this);
    m_labelTitle->setStyleSheet("font-weight: bold");
    m_labelTitle->hide(); // Hide by default until text is set

    m_labelError = new QLabel(this);
    m_labelError->setStyleSheet("color: red; font-size: 11px;");
    m_labelError->hide();

    m_layout->addWidget(m_labelTitle);
    // The input widget will be added here by subclasses
    m_layout->addWidget(m_labelError);
}

void InputBase::setInputWidget(QWidget *widget) {
    // Insert the input widget between the title and error labels
    m_layout->insertWidget(1, widget);
}

QString InputBase::labelText() const {
    return m_labelTitle->text();
}

void InputBase::setLabelText(const QString &text) {
    m_labelTitle->setText(text);
    m_labelTitle->setVisible(!text.isEmpty());
}

void InputBase::setError(const QString &errorText) {
    m_labelError->setText(errorText);
    m_labelError->setVisible(!errorText.isEmpty());
}

void InputBase::clearError() {
    m_labelError->clear();
    m_labelError->hide();
}
