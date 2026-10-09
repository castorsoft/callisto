#include "DatabaseManager.h"

#include <QSqlError>
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::~DatabaseManager()
{
    disconnect();
}

QString DatabaseManager::connectionNameForCurrentThread() const
{
    QThread *curThread = QThread::currentThread();
    if (!QCoreApplication::instance() || curThread == QCoreApplication::instance()->thread()) {
        return QSqlDatabase::defaultConnection;
    }
    return QString("thread_conn_%1").arg(reinterpret_cast<quintptr>(curThread), 0, 16);
}

bool DatabaseManager::connectToDatabase(const QString &host, int port, const QString &dbName, const QString &user, const QString &password)
{
    QMutexLocker locker(&m_mutex);
    m_host = host;
    m_port = port;
    m_dbName = dbName;
    m_user = user;
    m_password = password;
    m_configured = true;

    QSqlDatabase db;
    if (QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        db = QSqlDatabase::database(QSqlDatabase::defaultConnection);
    } else {
        db = QSqlDatabase::addDatabase("QPSQL");
    }

    db.setHostName(m_host);
    db.setPort(m_port);
    db.setDatabaseName(m_dbName);
    db.setUserName(m_user);
    db.setPassword(m_password);

    if (!db.open()) {
        qCritical() << "[DatabaseManager] Erro ao conectar ao PostgreSQL: " << db.lastError().text();
        m_configured = false;
        return false;
    }

    m_activeConnections.insert(QSqlDatabase::defaultConnection);
    qInfo() << "[DatabaseManager] Conexao com o PostgreSQL estabelecida com sucesso.";
    return true;
}

void DatabaseManager::disconnect()
{
    QMutexLocker locker(&m_mutex);
    for (const QString &connName : m_activeConnections) {
        if (QSqlDatabase::contains(connName)) {
            {
                QSqlDatabase db = QSqlDatabase::database(connName, false);
                if (db.isOpen()) {
                    db.close();
                }
            }
            QSqlDatabase::removeDatabase(connName);
        }
    }
    m_activeConnections.clear();
    m_configured = false;
    qInfo() << "[DatabaseManager] Conexoes encerradas.";
}

QSqlDatabase DatabaseManager::database() const
{
    if (!m_configured) {
        qCritical() << "[DatabaseManager] Tentativa de obter conexao antes de connectToDatabase().";
        return QSqlDatabase();
    }

    QString connName = connectionNameForCurrentThread();

    QMutexLocker locker(&m_mutex);

    if (QSqlDatabase::contains(connName)) {
        QSqlDatabase db = QSqlDatabase::database(connName, false);
        if (db.isOpen()) {
            return db;
        }
        if (db.open()) {
            return db;
        }
        {
            db.close();
        }
        QSqlDatabase::removeDatabase(connName);
        m_activeConnections.remove(connName);
    }

    QSqlDatabase db = (connName == QSqlDatabase::defaultConnection)
        ? QSqlDatabase::addDatabase("QPSQL")
        : QSqlDatabase::addDatabase("QPSQL", connName);

    db.setHostName(m_host);
    db.setPort(m_port);
    db.setDatabaseName(m_dbName);
    db.setUserName(m_user);
    db.setPassword(m_password);

    if (!db.open()) {
        qCritical() << "[DatabaseManager] Erro ao abrir conexao PostgreSQL para" << connName << ":" << db.lastError().text();
    } else {
        qInfo() << "[DatabaseManager] Nova conexao criada com sucesso para" << connName;
        m_activeConnections.insert(connName);

        QThread *curThread = QThread::currentThread();
        if (QCoreApplication::instance() && curThread != QCoreApplication::instance()->thread()) {
            QObject::connect(curThread, &QThread::finished, [connName]() {
                if (QSqlDatabase::contains(connName)) {
                    {
                        QSqlDatabase tdb = QSqlDatabase::database(connName, false);
                        if (tdb.isOpen()) {
                            tdb.close();
                        }
                    }
                    QSqlDatabase::removeDatabase(connName);
                    qInfo() << "[DatabaseManager] Conexao removida ao finalizar thread:" << connName;
                }
            });
        }
    }

    return db;
}
