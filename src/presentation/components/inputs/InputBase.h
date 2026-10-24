#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class InputBase : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QString labelText READ labelText WRITE setLabelText)

public:
    explicit InputBase(QWidget *parent = nullptr);

    QString labelText() const;
    void setLabelText(const QString &text);

    void setError(const QString &errorText);
    void clearError();

protected:
    void setInputWidget(QWidget *widget);

    QVBoxLayout *m_layout;
    QLabel *m_labelTitle;
    QLabel *m_labelError;
};
