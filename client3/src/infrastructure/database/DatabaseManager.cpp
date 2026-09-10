#include "DatabaseManager.h"

#include <QSqlError>

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::connectToDatabase(const QString &host, int port, const QString &dbName, const QString &user, const QString &password)
{
    if (m_db.isOpen()) {
        return true;
    }

    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(host);
    m_db.setPort(port);
    m_db.setDatabaseName(dbName);
    m_db.setUserName(user);
    m_db.setPassword(password);

    if (!m_db.open()) {
        qCritical() << "[DatabaseManager] Erro ao conectar ao PostgreSQL: " << m_db.lastError().text();
        return false;
    }

    qInfo() << "[DatabaseManager] Conexao com o PostgreSQL estabelecida com sucesso.";
    return true;
}

void DatabaseManager::disconnect() {
    if (m_db.isOpen()) {
        m_db.close();
        qInfo() << "[DatabaseManager] Conexao encerrada.";
    }
}

QSqlDatabase DatabaseManager::database() const
{
    return m_db;
}
