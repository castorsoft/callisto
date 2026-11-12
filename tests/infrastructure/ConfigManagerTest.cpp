#include <gtest/gtest.h>
#include <QFile>
#include <QTemporaryDir>

#include "infrastructure/config/ConfigManager.h"

TEST(ConfigManagerTest, CreatesDefaultConfigWhenFileDoesNotExist) {
    QTemporaryDir tempDir;
    ASSERT_TRUE(tempDir.isValid());

    QString configPath = tempDir.filePath("default_config.json");
    EXPECT_FALSE(QFile::exists(configPath));

    auto &cfg = ConfigManager::instance();
    ASSERT_TRUE(cfg.load(configPath));
    EXPECT_TRUE(QFile::exists(configPath));

    EXPECT_EQ(cfg.dbHost(), "localhost");
    EXPECT_EQ(cfg.dbPort(), 5432);
    EXPECT_EQ(cfg.dbName(), "callisto");
    EXPECT_EQ(cfg.dbUser(), "postgres");
    EXPECT_EQ(cfg.dbPassword(), "secret");
    ASSERT_EQ(cfg.searchPaths().size(), 1);
    EXPECT_EQ(cfg.searchPaths().first(), "./data/files/");
}

TEST(ConfigManagerTest, SaveAndReloadCustomDatabaseAndSearchConfig) {
    QTemporaryDir tempDir;
    ASSERT_TRUE(tempDir.isValid());

    QString configPath = tempDir.filePath("custom_config.json");
    auto &cfg = ConfigManager::instance();
    ASSERT_TRUE(cfg.load(configPath));

    cfg.setDatabaseConfig("192.168.1.50", 5433, "dfe_prod", "admin_user", "s3cr3t!");
    cfg.setServiceSearchConfig({"/var/dfe/xmls", "/mnt/nfes"});
    ASSERT_TRUE(cfg.save());

    // Sobrescreve em memória para garantir que o load recarrega do disco
    cfg.setDatabaseConfig("other", 1111, "other", "other", "other");
    cfg.setServiceSearchConfig({});

    ASSERT_TRUE(cfg.load(configPath));
    EXPECT_EQ(cfg.dbHost(), "192.168.1.50");
    EXPECT_EQ(cfg.dbPort(), 5433);
    EXPECT_EQ(cfg.dbName(), "dfe_prod");
    EXPECT_EQ(cfg.dbUser(), "admin_user");
    EXPECT_EQ(cfg.dbPassword(), "s3cr3t!");

    QStringList paths = cfg.searchPaths();
    ASSERT_EQ(paths.size(), 2);
    EXPECT_EQ(paths.at(0), "/var/dfe/xmls");
    EXPECT_EQ(paths.at(1), "/mnt/nfes");
}

TEST(ConfigManagerTest, FailsOnMalformedOrNonObjectJson) {
    QTemporaryDir tempDir;
    ASSERT_TRUE(tempDir.isValid());

    QString invalidJsonPath = tempDir.filePath("invalid.json");
    {
        QFile file(invalidJsonPath);
        ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
        file.write("{ invalid json ");
    }

    auto &cfg = ConfigManager::instance();
    EXPECT_FALSE(cfg.load(invalidJsonPath));

    QString arrayJsonPath = tempDir.filePath("array.json");
    {
        QFile file(arrayJsonPath);
        ASSERT_TRUE(file.open(QIODevice::WriteOnly | QIODevice::Text));
        file.write("[1, 2, 3]");
    }
    EXPECT_FALSE(cfg.load(arrayJsonPath));
}

