#pragma once

#include "InputBase.h"
#include <QButtonGroup>
#include <QRadioButton>
#include <QVariant>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QList>

class InputRadioGroup : public InputBase {
    Q_OBJECT
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)

public:
    explicit InputRadioGroup(QWidget *parent = nullptr);

    void addOption(const QString &text, const QVariant &userData = QVariant());
    void clear();

    int currentIndex() const;
    QVariant currentData() const;
    void setCurrentIndex(int index);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

signals:
    void currentIndexChanged(int index);

private slots:
    void onButtonToggled(QAbstractButton *button, bool checked);

private:
    QWidget *m_containerWidget;
    QBoxLayout *m_containerLayout;
    QButtonGroup *m_buttonGroup;
    QList<QVariant> m_userDataList;
};
