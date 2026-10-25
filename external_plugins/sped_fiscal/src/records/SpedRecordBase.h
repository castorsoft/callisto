#pragma once

#include <QString>
#include <QDate>
#include <QStringList>

class SpedRecordBase {
public:
    explicit SpedRecordBase(const QString &regName);
    virtual ~SpedRecordBase() = default;

    QString registerName() const { return m_regName; }

    virtual QString buildLine() const = 0;

protected:
    static QString formatString(const QString &val);
    static QString formatDate(const QDate &date);
    static QString formatDecimal(double val, int decimals = 2, bool allowEmptyIfZero = false);
    static QString formatInt(int val);
    static QString assembleLine(const QStringList &fields);

    QString m_regName;
};

