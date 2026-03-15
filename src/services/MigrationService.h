#pragma once

#include "../repositories/MigrationRepository.h"

#include <memory>

#include <QString>

class MigrationService {
public:
    static MigrationService& instance() {
        static MigrationService inst;
        return inst;
    }

    std::shared_ptr<Migrations> getMigration(QString &migration);
    bool insert(std::shared_ptr<Migrations> migration);

    // Regra de negócio: Registra uma nova migração apenas se for válida
    // bool registerMigration(Migration &m);

    // std::vector<Migration> listAll();

private:
    MigrationService() = default;
};