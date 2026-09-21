#include "application/use_cases/ProcessFile.h"

#include "infrastructure/background/ServiceManager.h"
#include "infrastructure/background/services/DirectoryWatcherWorker.h"
#include "infrastructure/background/services/ScheduleFilesForProcessing.h"
#include "infrastructure/config/ConfigManager.h"
#include "infrastructure/database/DatabaseManager.h"
#include "infrastructure/database/MigrationRunner.h"

#include "plugins/PluginManager.h"

#ifdef HEADLESS_MODE
#include <QCoreApplication>
#else
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include "presentation/views/MainWindow.h"
#endif

#include <QEventLoop>
#include <QTimer>

void startDirectoryWatcherService() {
    auto watcherService = new DirectoryWatcherWorker(ConfigManager::instance().searchPaths()[0]);
    ServiceManager::instance().registerService(watcherService);

    auto scheduler = new ScheduleFilesForProcessing();
    ServiceManager::instance().registerService(scheduler);

    auto fileProcessor = new ProcessFile();

    QObject::connect(scheduler, &ScheduleFilesForProcessing::requestFileProcess, fileProcessor, &ProcessFile::execute, Qt::QueuedConnection);
    QObject::connect(watcherService, &DirectoryWatcherWorker::fileAdded, scheduler, &ScheduleFilesForProcessing::onFileAdded, Qt::QueuedConnection);

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
#ifdef HEADLESS_MODE
    QCoreApplication a(argc, argv);
#else
    QApplication a(argc, argv);
#endif
    qRegisterMetaType<Arquivo>("Arquivo");

#ifndef HEADLESS_MODE
    QPixmap q("://assets/splash_620x300.png");
    QSplashScreen splash(q);

    splash.show();
    a.processEvents();
#endif

    // Salva os logs na pasta
    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz ttt} %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] %{file}:%{line} - %{message}");
    qInstallMessageHandler(logToFile);

    auto showMessage = [&](const QString &msg) {
#ifndef HEADLESS_MODE
        splash.showMessage(msg);
#endif
        qInfo() << msg;
    };

    // Leitura do arquivo config.json
    showMessage("Carregando configurações.");
    ConfigManager &config = ConfigManager::instance();
    if (!config.load(a.applicationDirPath() + "/config.json")) {
        qCritical() << "Erro ao incializar as configuracoes da aplicacao. A aplicacao sera encerrada.";
        return -1;
    }

    // Comunicação com o BD
    showMessage("Estabelecendo a comunicação com o banco de dados.");
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
    showMessage("Verificando versão do banco de dados.");
    MigrationRunner runner(dbMgr.database());
    QString migrationsPath = a.applicationDirPath() + "/migrations";

    if (!runner.runPendingMigrations(migrationsPath)) {
        qCritical() << "Falha ao executar as migrations do banco de dados. A aplicacao sera encerrada.";
        return -1;
    }

    // Carrega o sistema de plugins
    showMessage("Carregando plugins.");
    PluginManager &pluginMgr = PluginManager::instance();
    pluginMgr.loadPlugins(a.applicationDirPath() + "/bin/plugins");

    showMessage("Inicializando serviços");
    // Adicionar demais services aqui
    startDirectoryWatcherService();

    ServiceManager::instance().startAll();

    showMessage("O sistema está sendo iniciado.");

#ifndef HEADLESS_MODE
    MainWindow w;
    w.show();
    splash.finish(&w);
    return QApplication::exec();
#else
    return QCoreApplication::exec();
#endif
}
