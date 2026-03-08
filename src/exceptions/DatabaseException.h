#pragma once

#include <QException>
#include <QString>

class DatabaseException: public QException {
public:
    explicit DatabaseException(const QString message): m_message(message) {}

    QString message() const { return m_message; }

    void raise() const override { throw *this; }

    DatabaseException *clone() const override { return new DatabaseException(*this); }

    const char *what() const noexcept override { return m_message.toStdString().c_str(); }

private:
    QString m_message;
};
