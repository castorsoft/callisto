#include "forms/MainWindow.h"
#include "core/DatabaseManager.h"
#include "core/ConfigManager.h"
#include "core/MigrationRunner.h"
#include "core/PluginManager.h"
#include "core/ServiceManager.h"
#include "core/services/DirectoryWatcherWorker.h"

#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QEventLoop>
#include <QTimer>

void startDirectoryWatcherService() {
    auto watcherService = new DirectoryWatcherWorker(ConfigManager::instance().searchPaths()[0]);
    ServiceManager::instance().RegisterService(watcherService);
}

void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message = qFormatLogMessage(type, context, msg);
    static FILE *f = fopen("application.log", "a");
    fprintf(f, "%s\n", qPrintable(message));
    fflush(f);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap q("://Resources/splash_620x300.png");
    QSplashScreen splash(q);

    splash.show();
    a.processEvents();

    // Salva os logs na pasta
    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz ttt} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");
    qInstallMessageHandler(logToFile);

    // Leitura do arquivo config.json
    splash.showMessage("Carregando configurações.");
    ConfigManager &config = ConfigManager::instance();
    if (!config.load(a.applicationDirPath() + "/config.json")) {
        qCritical() << "Erro ao incializar as configuracoes da aplicacao. A aplicacao sera encerrada.";
        return -1;
    }

    // Comunicação com o BD
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

    // Migrations do BD
    splash.showMessage("Verificando versão do banco de dados.");
    MigrationRunner runner(dbMgr.database());
    QString migrationsPath = a.applicationDirPath() + "/migrations";

    if (!runner.runPendingMigrations(migrationsPath)) {
        qCritical() << "Falha ao executar as migrations do banco de dados. A aplicacao sera encerrada.";
        return -1;
    }

    // Carrega o sistema de plugins
    splash.showMessage("Carregando plugins.");
    PluginManager &pluginMgr = PluginManager::instance();
    pluginMgr.loadPlugins(a.applicationDirPath() + "/bin/plugins");

    splash.showMessage("Inicializando serviços");
    // Adicionar demais services aqui
    startDirectoryWatcherService();

    ServiceManager::instance().startAll();

    splash.showMessage("O sistema está sendo iniciado.");

    MainWindow w;
    w.show();
    splash.finish(&w);
    return QApplication::exec();
}
