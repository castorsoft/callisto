#pragma once

#include <drogon/drogon.h>

#include <QString>

#include "../models/Migrations.h"

using namespace drogon_model::dfe;

class MigrationRepository {
public:
    static MigrationRepository& instance() {
        static MigrationRepository inst;
        return inst;
    }

    // std::optional<Migration> getById(int id);
    // std::vector<Migration> getAll(const drogon::orm::Criteria &criteria);
    std::shared_ptr<Migrations> findByMigration(const QString &migration);
    void save(std::shared_ptr<Migrations> migration);
    void update(const std::shared_ptr<Migrations> migration);

private:
    MigrationRepository() = default;
    drogon::orm::Mapper<Migrations> getMapper() {
        return drogon::orm::Mapper<Migrations>(drogon::app().getDbClient());
    }
};