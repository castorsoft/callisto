#include "InputRadioGroup.h"

InputRadioGroup::InputRadioGroup(QWidget *parent) : InputBase(parent) {
    m_containerWidget = new QWidget(this);
    m_containerLayout = new QHBoxLayout(m_containerWidget);
    m_containerLayout->setContentsMargins(0, 0, 0, 0);
    m_containerLayout->setSpacing(12);

    m_buttonGroup = new QButtonGroup(this);
    m_buttonGroup->setExclusive(true);

    connect(m_buttonGroup, &QButtonGroup::buttonToggled, this, &InputRadioGroup::onButtonToggled);

    setInputWidget(m_containerWidget);
}

void InputRadioGroup::addOption(const QString &text, const QVariant &userData) {
    int id = m_buttonGroup->buttons().size();
    QRadioButton *radio = new QRadioButton(text, m_containerWidget);
    m_containerLayout->addWidget(radio);
    m_buttonGroup->addButton(radio, id);
    m_userDataList.append(userData);

    if (id == 0) {
        radio->setChecked(true); // Seleciona o primeiro por padrão
    }
}

void InputRadioGroup::clear() {
    for (auto button : m_buttonGroup->buttons()) {
        m_buttonGroup->removeButton(button);
        m_containerLayout->removeWidget(button);
        button->deleteLater();
    }
    m_userDataList.clear();
}

int InputRadioGroup::currentIndex() const {
    return m_buttonGroup->checkedId();
}

QVariant InputRadioGroup::currentData() const {
    int id = currentIndex();
    if (id >= 0 && id < m_userDataList.size()) {
        return m_userDataList.at(id);
    }
    return QVariant();
}

void InputRadioGroup::setCurrentIndex(int index) {
    if (auto btn = m_buttonGroup->button(index)) {
        btn->setChecked(true);
        emit currentIndexChanged(index);
    }
}

Qt::Orientation InputRadioGroup::orientation() const {
    return (qobject_cast<QHBoxLayout*>(m_containerLayout)) ? Qt::Horizontal : Qt::Vertical;
}

void InputRadioGroup::setOrientation(Qt::Orientation orientation) {
    if (this->orientation() == orientation) return;

    QBoxLayout *newLayout = nullptr;
    if (orientation == Qt::Horizontal) {
        newLayout = new QHBoxLayout();
    } else {
        newLayout = new QVBoxLayout();
    }
    newLayout->setContentsMargins(0, 0, 0, 0);
    newLayout->setSpacing(12);

    for (auto btn : m_buttonGroup->buttons()) {
        newLayout->addWidget(btn);
    }

    delete m_containerLayout;
    m_containerLayout = newLayout;
    m_containerWidget->setLayout(m_containerLayout);
}

void InputRadioGroup::onButtonToggled(QAbstractButton *button, bool checked) {
    if (checked) {
        emit currentIndexChanged(m_buttonGroup->id(button));
    }
}
