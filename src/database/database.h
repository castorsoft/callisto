#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager {
public:
    static DatabaseManager& instance();

    bool open(const QString& driver = "QMYSQL",
              const QString& host = "localhost",
              const QString& port = "5432",
              const QString& dbname = "",
              const QString& user = "",
              const QString& pass = "");
    void close();

    QSqlDatabase database() const; // conexão global (atenção a threads)

private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
};

#endif // DATABASE_H