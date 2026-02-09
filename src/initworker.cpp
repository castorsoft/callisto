#include "initworker.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QThread>

InitWorker::InitWorker(QObject *parent) : QObject(parent) {}

void InitWorker::run() {
    emit progress("Inicializando conexão com o banco...");

    // Cria conexão nomeada para garantir remoção correta
    const QString connName = "dfe_init_conn";
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", connName);
    db.setHostName("localhost");
    db.setDatabaseName("dfe");
    db.setUserName("user");
    db.setPassword("password");
    // ajustar parâmetros conforme sua configuração

    if (!db.open()) {
        QString err = db.lastError().text();
        QSqlDatabase::removeDatabase(connName);
        emit finished(false, QString("Erro ao conectar ao BD: %1").arg(err));
        return;
    }

    emit progress("Conexão estabelecida. Verificando migrações...");

    // TODO: chamar sua MigrationManager aqui.
    // Exemplo (ajuste para sua API):
    // try {
    //     dfe::DB wrapper(db); // se tiver wrapper que aceita QSqlDatabase
    //     dfe::MigrationManager migrator(wrapper);
    //     migrator.migrate("./migrations");
    // } catch (const std::exception &e) {
    //     db.close();
    //     QSqlDatabase::removeDatabase(connName);
    //     emit finished(false, QString("Migração falhou: %1").arg(e.what()));
    //     return;
    // }

    QThread::sleep(1); // breve pausa visual (opcional)
    emit progress("Carregando plugins...");

    // TODO: carregar plugins (PluginManager) aqui.
    // Exemplo:
    // dfe::PluginManager pm;
    // pm.loadDirectory("./plugins");

    QThread::sleep(1);

    // Finalizar
    db.close();
    QSqlDatabase::removeDatabase(connName);

    emit progress("Inicialização concluída.");
    emit finished(true, QString());
}