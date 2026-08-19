#include "MigrationRunner.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QIODevice>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QApplication>
#include <QSqlRecord>

MigrationRunner::MigrationRunner(QSqlDatabase db): m_db(db) {}

bool MigrationRunner::runPendingMigrations(const QString &migrationsDirPath) {
    if (!m_db.isOpen()) {
        qCritical() << "[MigrationRunner] O banco de dados nao esta aberto.";
        return false;
    }

    if (!ensureMigrationsTableExists()) {
        return false;
    }

    QDir dir(migrationsDirPath);
    if (!dir.exists()) {
        qCritical() << "[MigrationRunner] Diretorio de migrations nao encontrado: '" << migrationsDirPath << "'";
        return false;
    }

    QStringList filters;
    filters << "*.sql";
    dir.setNameFilters(filters);
    dir.setSorting(QDir::Name);

    QFileInfoList fileList = dir.entryInfoList();
    QStringList executedMigrations = getExecutedMigrations();

    int nextBatch = getNextBatchNumber();
    int ranCount = 0;

    for (const QFileInfo &fileInfo: fileList) {
        QString fileName = fileInfo.fileName();
        if (executedMigrations.contains(fileName)) {
            continue;
        }
        qInfo() << "[MigrationRunner] Executando nova migration: " << fileName;
        if (!executeMigrationFile(fileInfo.absoluteFilePath(), fileName, nextBatch)) {
            qCritical() << "[MigrationRunner] Abortando migrations devido a um erro.";
            return false;
        }
        QApplication::processEvents();
        ranCount++;
    }

    if (ranCount == 0) {
        qInfo() << "[MigrationRunner] Nenhuma migration pendente.";
    } else {
        qInfo() << "[MigrationRunner] " << ranCount << " migration(s) executada(s) no Batch " << nextBatch;
    }

    return true;
}

bool MigrationRunner::ensureMigrationsTableExists() {
    QSqlQuery query(m_db);
    query.prepare("SELECT EXISTS (SELECT FROM information_schema.tables WHERE table_schema = :table_schema AND table_name = :table_name)");
    query.bindValue(":table_schema", "public");
    query.bindValue(":table_name", "migrations");
    query.exec();
    if (query.next() && !query.value(query.record().indexOf("exists")).toBool()) {
        QString sql = R"(
            CREATE TABLE IF NOT EXISTS migrations (
                id SERIAL PRIMARY KEY,
                migration VARCHAR(255) NOT NULL,
                batch INT NOT NULL,
                executed_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
            );
        )";

        query.clear();
        if (!query.exec(sql)) {
            qCritical() << "[MigrationRunner] Erro ao criar/verificar a tabela 'migrations': " << query.lastError().text();
            return false;
        }
    }

    return true;
}

int MigrationRunner::getNextBatchNumber() {
    QSqlQuery query(m_db);
    query.exec("SELECT MAX(batch) FROM migrations");
    if (query.next() && !!query.value(0).isNull()) {
        return query.value(0).toInt() + 1;
    }
    return 1;
}

QStringList MigrationRunner::getExecutedMigrations() {
    QStringList executed;
    QSqlQuery query("SELECT migration FROM migrations ORDER BY id ASC", m_db);
    while (query.next()) {
        executed.append(query.value(0).toString());
    };
    return executed;
}

bool MigrationRunner::executeMigrationFile(const QString &filePath, const QString &fileName, int batch) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "[MigrationRunner] Nao foi possível abrir o arquivo '" << filePath << "'";
        return false;
    }

    QString sqlContent = QString::fromUtf8(file.readAll()).trimmed();
    file.close();

    if (sqlContent.isEmpty()) {
        qWarning() << "[MigrationRunner] Arquivo de migration vazio '" << filePath << "'";
        return true;
    }

    if (!m_db.transaction()) {
        qCritical() << "MigrationRunner Erro ao iniciar transacao para '" << filePath << "'";
        return false;
    }

    QSqlQuery query(m_db);
    if (!query.exec(sqlContent)) {
        qCritical() << "[MigrationRunner] Erro ao executar DDL da migration '" << filePath << "': " << query.lastError().text();
        m_db.rollback();
        return false;
    }

    QSqlQuery recordQuery(m_db);
    recordQuery.prepare("INSERT INTO migrations (migration, batch) VALUES (:migration, :batch)");
    recordQuery.bindValue(":migration", fileName);
    recordQuery.bindValue(":batch", batch);

    if (!recordQuery.exec()) {
        qCritical() << "[MigrationRunner] Falha ao registrar a migration " << fileName << ": " << recordQuery.lastError().text();
        m_db.rollback();
        return false;
    }

    if (!m_db.commit()) {
        qCritical() << "[migrationRunner] Erro ao dar commit na transacao para " << fileName;
        return false;
    }

    qInfo() << "[MigrationRunner] Executado com sucesso: " << fileName << " (Batch: " << batch << ")";
    return true;
}
