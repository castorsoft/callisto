#include <iostream>
#include <exception>

#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QSqlQuery>
#include <QPixmap>
#include <QSqlError>
#include <QDebug>

#include "forms/MainWindow.h"
#include "database/database.h"
#include "utilities/JsonUtils.h"

void runStartUpFunctions();
void establishDatabaseConnection();
void runMigrations();

int main(int argc, char *argv[]) {

    qDebug() << "Iniciando aplicação";
    QApplication app(argc, argv);

    MainWindow window;

    try {
        runStartUpFunctions();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    window.show();

    return app.exec();
}

void runStartUpFunctions()
{
    qDebug() << "Executando funções de inicialização";
    establishDatabaseConnection();
    runMigrations();
}

void establishDatabaseConnection() {
    qDebug() << "Estabelecendo comunicação com o banco de dados";
    QString val;
    QString host, database, username, password;
    int port;

    {
        QFile file("./config.json");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cerr << "Failed to open config file" << std::endl;
        }
        val = file.readAll();
    }

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());

    QJsonObject root = doc.object();

    host = JsonUtils::getValue<QString>(root, "database.host", QString("localhost"));
    port = JsonUtils::getValue<int>(root, "database.port", 5432);
    database = JsonUtils::getValue<QString>(root, "database.name", QString(""));
    username = JsonUtils::getValue<QString>(root, "database.username", QString(""));
    password = "yamete";

    if (DatabaseManager::instance().open("QPSQL", host, port, database, username, password)) {
        qDebug() << "Conexão estabelecida com sucesso";
    } else {
        qDebug() << "Erro: " + DatabaseManager::instance().lastError().text();
    }
}

void runMigrations() {
    qDebug() << "Executando migrations";

    QDir dir("./migrations");
    QStringList filters;
    filters << "*.sql";
    dir.setNameFilters(filters);
    QStringList migrationFiles = dir.entryList(QDir::Files, QDir::Name);

    qDebug() << "Arquivos para serem rodados: " << migrationFiles.count();
    for (const QString &fileName : migrationFiles) {
        QFile file(dir.filePath(fileName));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::cerr << "Failed to open migration file: " << fileName.toStdString() << std::endl;
        }
        QString sql = file.readAll();
        QSqlQuery query(DatabaseManager::instance().database());
        if (!query.exec(sql)) {
            qDebug() << "Não executou";
            std::cout << "Failed to execute migration: " << fileName.toStdString() << std::endl;
            // throw std::exception("Failed to execute migration " + fileName.toStdString() + ": " + query.lastError().text().toStdString());
        } else {
            // MigrationRepository::create(fileName.toStdString());
            qDebug() << "Successfully ran migration: " + fileName.toStdString();
        }
    }
    qDebug() << "Last error: " + DatabaseManager::instance().lastError().text();
}