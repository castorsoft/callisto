#include "forms/mainwindow.h"
#include "data/databasemanager.h"
#include "data/configmanager.h"
#include "data/migrationrunner.h"

#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QEventLoop>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap q("://Resources/splash_620x300.png");
    if (q.isNull()) qDebug("IsNull");
    QSplashScreen splash(q);

    splash.show();
    a.processEvents();

    splash.showMessage("Carregando configurações.");
    ConfigManager &config = ConfigManager::instance();
    if (!config.load(a.applicationDirPath() + "/config.json")) {
        qCritical() << "Erro ao incializar as configuracoes da aplicacao. A aplicacao sera encerrada.";
        return -1;
    }

    splash.showMessage("Estabelecendo a comunicação com o banco de dados.");
    DatabaseManager &dbMgr = DatabaseManager::instance();
    if (!dbMgr.connectToDatabase(
        config.dbHost(),
        config.dbPort(),
        config.dbName(),
        config.dbUser(),
        config.dbPassword()
    )) {
        qCritical() << "Nao foi possivel estabelecer a conexao com o banco de dados. A aplicacao sera encerrada.";
        return -1;
    }

    splash.showMessage("Verificando versão do banco de dados.");
    MigrationRunner runner(dbMgr.database());
    QString migrationsPath = a.applicationDirPath() + "/migrations";

    if (!runner.runPendingMigrations(migrationsPath)) {
        qCritical() << "Falha ao executar as migrations do banco de dados. A aplicacao sera encerrada.";
        return -1;
    }

    splash.showMessage("O sistema está sendo iniciado.");

    a.setStyle("")
    MainWindow w;
    w.show();
    splash.finish(&w);
    return QApplication::exec();
}
