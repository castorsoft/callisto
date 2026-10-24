#include "SpedRecordBase.h"

SpedRecordBase::SpedRecordBase(const QString &regName)
    : m_regName(regName)
{
}

QString SpedRecordBase::formatString(const QString &val)
{
    QString sanitized = val;
    sanitized.remove('|');
    sanitized.remove('\r');
    sanitized.remove('\n');
    return sanitized.trimmed();
}

QString SpedRecordBase::formatDate(const QDate &date)
{
    if (!date.isValid()) return QString();
    return date.toString("ddMMyyyy");
}

QString SpedRecordBase::formatDecimal(double val, int decimals, bool allowEmptyIfZero)
{
    if (allowEmptyIfZero && qAbs(val) < 0.000001) {
        return QString();
    }
    QString str = QString::number(val, 'f', decimals);
    str.replace('.', ',');
    return str;
}

QString SpedRecordBase::formatInt(int val)
{
    return QString::number(val);
}

QString SpedRecordBase::assembleLine(const QStringList &fields)
{
    return "|" + fields.join("|") + "|";
}

