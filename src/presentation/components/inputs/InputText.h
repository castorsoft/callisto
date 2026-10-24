#pragma once

#include "InputBase.h"
#include <QLineEdit>

class InputText : public InputBase {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString placeholderText READ placeholderText WRITE setPlaceholderText)

public:
    explicit InputText(QWidget *parent = nullptr);

    QString text() const;
    void setText(const QString &text);

    QString placeholderText() const;
    void setPlaceholderText(const QString &text);

    QLineEdit* lineEdit() const { return m_lineEdit; }

signals:
    void textChanged(const QString &text);

private:
    QLineEdit *m_lineEdit;
};
