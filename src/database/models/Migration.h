#pragma once

#include "Model.h"

class Migration : public Model<Migration> {
public:
    Migration() {};


    int id = -1;
    QString migration;

    QString tableName() const override { return "public.migrations"; }
    bool inserting() const { return this->id == -1; }

    void fill(const QSqlRecord &record) override {
        this->id = record.value("id").toInt();
        this->migration = record.value("migration").toString();
    }

    bool save() {
        QSqlQuery query(this->database());

        if (this->inserting()) {
            query.prepare(QString("INSERT INTO %1 (migration) VALUES (:migration)").arg(this->tableName()));
            qDebug() << query.lastError().text();
        } else {
            query.prepare(QString("UPDATE %1 SET migration = :migration WHERE id = :id").arg(this->tableName()));
            qDebug() << query.lastError().text();
            query.bindValue(":id", this->id);
        }
        query.bindValue(":migration", this->migration);
        auto ok = query.exec();
        this->id = query.lastInsertId().toInt();
        if (!ok) {
            qDebug() << "Erro ao salvar a migração: " + query.lastError().text();
        }
        return ok;
    }
};