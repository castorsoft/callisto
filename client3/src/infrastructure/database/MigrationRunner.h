#pragma once

#include <QtSql/QSqlDatabase>
#include <QString>

class MigrationRunner {
public:
    explicit MigrationRunner(QSqlDatabase db);
    /**
     * @brief Executa as migrations pendentes.
     * @param migrationsDirPath Caminho da pasta com os arquivos .sql a serem rodados.
     * @return true caso sucesso, false em caso de falha.
     */
    bool runPendingMigrations(const QString &migrationsDirPath);
private:
    QSqlDatabase m_db;

    bool ensureMigrationsTableExists();
    int getNextBatchNumber();
    QStringList getExecutedMigrations();
    bool executeMigrationFile(const QString &filePath, const QString &fileName, int batch);
};