#include "database.h"
#include <QSqlDatabase>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager inst;
    return inst;
}

DatabaseManager::DatabaseManager() : m_db(QSqlDatabase()) {}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::open(const QString& driver, const QString& host,
                           const int& port, const QString& dbname , const QString& user, const QString& pass)
{
    if (m_db.isValid() && m_db.isOpen()) return true;
    m_db = QSqlDatabase::addDatabase(driver, "app_connection");
    m_db.setHostName(host);
    m_db.setDatabaseName(dbname);
    m_db.setPort(port);
    m_db.setUserName(user);
    m_db.setPassword(pass);

    return m_db.open();
}



void DatabaseManager::close() {
    if (m_db.isOpen()) m_db.close();
    QSqlDatabase::removeDatabase("app_connection");
}

QSqlError DatabaseManager::lastError()
{
    return m_db.lastError();
}

QSqlDatabase DatabaseManager::database() const {
    if (QSqlDatabase::contains("app_connection"))
        return QSqlDatabase::database("app_connection");
    return m_db;
}