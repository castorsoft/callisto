#pragma once

#include "InputBase.h"
#include <QCheckBox>

class InputCheckBox : public InputBase {
    Q_OBJECT
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY toggled)
    Q_PROPERTY(QString text READ text WRITE setText)

public:
    explicit InputCheckBox(QWidget *parent = nullptr);

    bool isChecked() const;
    void setChecked(bool checked);

    QString text() const;
    void setText(const QString &text);

    QCheckBox* checkBox() const { return m_checkBox; }

signals:
    void toggled(bool checked);

private:
    QCheckBox *m_checkBox;
};
