#include "MigrationService.h"
#include <filesystem>

// std::shared_ptr<Migration> MigrationService::getMigration(int id) {
//     auto result = MigrationRepository::instance().getById(id);
//     if (result) {
//         return std::make_shared<Migration>(std::move(*result));
//     }
//     return nullptr;
// }

// bool MigrationService::registerMigration(Migration &m) {
//     // Exemplo de Regra de Negócio:
//     // Verificar se o arquivo SQL mencionado na model realmente existe no servidor
//     if (!std::filesystem::exists(m.getValueOfFileName())) {
//         LOG_ERROR << "Arquivo SQL não encontrado: " << m.getValueOfFileName();
//         return false;
//     }

//     MigrationRepository::instance().save(m);
//     return true;
// }

// std::vector<Migration> MigrationService::listAll() {
//     return MigrationRepository::instance().getAll(drogon::orm::Criteria());
// }

std::shared_ptr<Migrations> MigrationService::getMigration(QString &migration)
{
    try
    {
        auto model = MigrationRepository::instance().findByMigration(migration);
        if (model != nullptr) {
            return std::make_shared<Migrations>(std::move(*model));
        }
    }
    catch(const std::exception& e) {}

    return nullptr;
}

bool MigrationService::insert(std::shared_ptr<Migrations> migration)
{
    MigrationRepository::instance().save(migration);
    return true;
}
