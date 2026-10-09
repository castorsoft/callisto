#pragma once

#include <QSqlDatabase>
#include <QString>
#include <QMutex>
#include <QSet>

class DatabaseManager {
public:
    static DatabaseManager& instance();

    bool connectToDatabase(const QString &host, int port, const QString &dbName, const QString &user, const QString &password);
    void disconnect();
    QSqlDatabase database() const;

private:
    DatabaseManager() = default;
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QString connectionNameForCurrentThread() const;

    QString m_host;
    int m_port{5432};
    QString m_dbName;
    QString m_user;
    QString m_password;
    bool m_configured{false};

    mutable QMutex m_mutex;
    mutable QSet<QString> m_activeConnections;
};