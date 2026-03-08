#pragma once

#include <QException>
#include <QString>

class FileNotFoundException: public QException {
public:
    explicit FileNotFoundException(const QString message): m_message(message) {}

    QString message() const { return m_message; }

    void raise() const override { throw *this; }

    FileNotFoundException *clone() const override { return new FileNotFoundException(*this); }

    const char *what() const noexcept override { return m_message.toStdString().c_str(); }

private:
    QString m_message;
};
