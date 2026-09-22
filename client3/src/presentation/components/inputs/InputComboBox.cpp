#include "InputComboBox.h"

InputComboBox::InputComboBox(QWidget *parent) : InputBase(parent) {
    m_comboBox = new QComboBox(this);
    setInputWidget(m_comboBox);

    connect(m_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &InputComboBox::currentIndexChanged);
}

void InputComboBox::addItem(const QString &text, const QVariant &userData) {
    m_comboBox->addItem(text, userData);
}

void InputComboBox::clear() {
    m_comboBox->clear();
}

QString InputComboBox::currentText() const {
    return m_comboBox->currentText();
}

QVariant InputComboBox::currentData() const {
    return m_comboBox->currentData();
}

int InputComboBox::currentIndex() const {
    return m_comboBox->currentIndex();
}

void InputComboBox::setCurrentIndex(int index) {
    m_comboBox->setCurrentIndex(index);
}
