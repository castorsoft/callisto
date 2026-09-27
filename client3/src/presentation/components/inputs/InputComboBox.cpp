#include "InputComboBox.h"
#include <QEvent>

InputComboBox::InputComboBox(QWidget *parent) : InputBase(parent) {
    m_comboBox = new QComboBox(this);
    setInputWidget(m_comboBox);

    connect(m_comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &InputComboBox::currentIndexChanged);
            
    m_comboBox->installEventFilter(this);
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

int InputComboBox::findIndex(const QVariant &data) {
    return this->m_comboBox->findData(data);
}

void InputComboBox::setCurrentIndex(int index) {
    m_comboBox->setCurrentIndex(index);
}

bool InputComboBox::eventFilter(QObject *obj, QEvent *event) {
    if (obj == m_comboBox && event->type() == QEvent::Wheel) {
        // Ignore o evento de scroll para evitar alteração de valor acidental
        return true;
    }
    return InputBase::eventFilter(obj, event);
}
