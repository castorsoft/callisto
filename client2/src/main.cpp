#include "forms/mainwindow.h"
#include "data/databasemanager.h"
#include "data/configmanager.h"
#include "data/migrationrunner.h"

#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QEventLoop>
#include <QTimer>


void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message = qFormatLogMessage(type, context, msg);
    static FILE *f = fopen("application.log", "a");
    fprintf(f, "%s\n", qPrintable(message));
    fflush(f);

    // if (originalHandler)
    //     originalHandler(type, context, msg);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap q("://Resources/splash_620x300.png");
    QSplashScreen splash(q);

    splash.show();
    a.processEvents();
    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz ttt} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");
    qInstallMessageHandler(logToFile);

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

    MainWindow w;
    w.show();
    splash.finish(&w);
    return QApplication::exec();
}
