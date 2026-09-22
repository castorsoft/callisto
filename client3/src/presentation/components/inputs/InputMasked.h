#pragma once

#include "InputBase.h"
#include <QLineEdit>

class InputMasked : public InputBase {
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString mask READ mask WRITE setMask)

public:
    explicit InputMasked(QWidget *parent = nullptr);

    QString text() const;
    QString mask() const;

    void setText(const QString &text);
    void setMask(const QString &mask);

signals:
    void textChanged(const QString &text);

private:
    QLineEdit *m_lineEdit;
};
