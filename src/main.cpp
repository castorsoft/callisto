#include <iostream>
#include <memory>

#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QSqlQuery>
#include <QPixmap>
#include <QSqlError>
#include <QDebug>
#include <string>
#include <QSqlDatabase>
#include <QException>

#include "exceptions/FileNotFoundException.h"
#include "exceptions/DatabaseException.h"

#include "forms/MainWindow.h"
// #include "database/database.h"
#include "database/models/Migration.h"
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
    catch(const QException& e) {
        qDebug() << "Erro ao executar os scripts: " << e.what();
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

    QString configFile = "config.json";
    {
        QFile file(configFile);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw DatabaseException("Arquivo não encontrado: " + configFile);
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

    auto db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPassword(password);

    if (db.open()) {
        qDebug() << "Conexão estabelecida com sucesso";
    } else {
        throw DatabaseException("Não foi possível se conectar ao banco de dados principal: " + db.lastError().text());
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
        auto migration = Migration::findByField("migration", fileName);

        if (migration == nullptr) {
            migration = std::make_unique<Migration>();
            migration->migration = fileName;

            QFile file(dir.filePath(fileName));

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                std::cerr << "Failed to open migration file: " << fileName.toStdString() << std::endl;
            }
            QString sql = file.readAll();

            QSqlQuery query;


            qDebug() << sql;
            if (!query.exec(sql)) {
                throw DatabaseException("Erro ao atualizar o banco no arquivo " + fileName + ": " + query.lastError().text());
            } else {
                qDebug() << "Arquivo executado com sucesso: " + fileName;
                migration->save();
            }
        }
        dir.remove(fileName);
    }
}