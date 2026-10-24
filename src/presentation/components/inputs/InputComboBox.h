#pragma once

#include "InputBase.h"
#include <QComboBox>
#include <QVariant>

class InputComboBox : public InputBase {
    Q_OBJECT

public:
    explicit InputComboBox(QWidget *parent = nullptr);

    void addItem(const QString &text, const QVariant &userData = QVariant());
    void clear();

    QString currentText() const;
    QVariant currentData() const;
    int currentIndex() const;
    int findIndex(const QVariant &data);

    void setCurrentIndex(int index);

    QComboBox* comboBox() const { return m_comboBox; }

signals:
    void currentIndexChanged(int index);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QComboBox *m_comboBox;
};
