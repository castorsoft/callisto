#include <memory>
#include <typeinfo>

#include "models/Migrations.h"
#include "exceptions/DatabaseException.h"

#include <drogon/drogon.h>
#include "services/MigrationService.h"
// #include <drogon/HttpAppFramework.h>
// #include <drogon/orm/Mapper.h>
#include <drogon/orm/DbConfig.h>
#include <drogon/orm/Exception.h>

#include <QDir>
#include <QFile>

using namespace drogon_model::dfe;
using namespace drogon::orm;

void runMigrations();

int main(int argc, char *argv[]) {

    drogon::app().registerBeginningAdvice([]() {
        try
        {
            runMigrations();
        }
        catch(const std::exception& e)
        {
            LOG_ERROR << typeid(e).name() << " - " << e.what();
        }

    });

    drogon::app().loadConfigFile("./server_config.json").setLogLevel(trantor::Logger::kDebug).addListener("0.0.0.0", 9000).setThreadNum(8).run();
}

void runMigrations()
{
    QDir dir("./migrations");
    QStringList filters;
    filters << "*.sql";
    dir.setNameFilters(filters);
    QStringList migrationFiles = dir.entryList(QDir::Files, QDir::Name);

    LOG_INFO << "Arquivos para serem rodados: " << migrationFiles.count();

    for (QString &fileName : migrationFiles) {
        LOG_INFO << fileName.toStdString() << ": Iniciando tratativas";
        auto migration = MigrationService::instance().getMigration(fileName);

        if (!migration) {
            LOG_DEBUG << fileName.toStdString() << ": Ainda não executado";
            QFile file(dir.filePath(fileName));

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                LOG_ERROR << fileName.toStdString() << ": Não foi possível ler o arquivo";
                throw DatabaseException("Não foi possível executar os migrations.");
            }

            QString sql = file.readAll();

            LOG_INFO << "Executando o arquivo " << fileName.toStdString();
            LOG_DEBUG << sql.toStdString();

            drogon::app().getDbClient()->execSqlSync(sql.toStdString());

            auto migration = std::make_shared<Migrations>();
            migration->setMigration(fileName.toStdString());

            if (MigrationService::instance().insert(migration)) {
                LOG_INFO << fileName.toStdString() << ": Executado com sucesso";
            }
        } else {
            LOG_INFO << fileName.toStdString() << ": Já executado anteriormente";
        }
        dir.remove(fileName);
    }
}