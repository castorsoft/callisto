#pragma once

#include <QSqlDatabase>
#include <QString>

class DatabaseManager {
public:
       static DatabaseManager& instance();

       bool connectToDatabase(const QString &host, int port, const QString &dbName, const QString &user, const QString &password);
       void disconnect();
       QSqlDatabase database() const;

private:
    DatabaseManager() = default;
    ~DatabaseManager() = default;

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
};