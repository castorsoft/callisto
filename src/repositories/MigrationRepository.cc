#include "MigrationRepository.h"

#include <drogon/HttpAppFramework.h>

#include <memory>

using namespace drogon::orm;

std::shared_ptr<Migrations> MigrationRepository::findByMigration(const QString &migration)
{
    try
    {
        auto model = MigrationRepository::getMapper().findOne(Criteria("migration", CompareOperator::EQ, migration.toStdString()));
        return std::make_shared<Migrations>(std::move(model));
    }
    catch(const UnexpectedRows& e)
    {
        return nullptr;
    }

}

void MigrationRepository::save(std::shared_ptr<Migrations> migration)
{
    auto mapper = getMapper();
    try {
        mapper.insert(*migration);
    } catch (const drogon::orm::DrogonDbException &e) {
        LOG_ERROR << "Erro ao salvar migration: " << e.base().what();
        throw; // Repassamos a exceção para o Service decidir o que fazer
    }
}

void MigrationRepository::update(const std::shared_ptr<Migrations> migration)
{
    auto mapper = getMapper();
    try {
        mapper.update(*migration);
    } catch (const drogon::orm::DrogonDbException &e) {
        LOG_ERROR << "Erro ao atualizar migration: " << e.base().what();
        throw;
    }
}
