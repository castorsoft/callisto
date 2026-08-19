#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QJsonObject>

class ConfigManager {
public:
    static ConfigManager& instance();

    bool load(const QString &filePath);

    bool save() const;

    // Configurações do banco de dados
    QString dbHost() const;
    int dbPort() const;
    QString dbName() const;
    QString dbUser() const;
    QString dbPassword() const;

    QStringList searchPaths() const;

    void setDatabaseConfig(const QString &host, int port, const QString &dbName, const QString &user, const QString &password);
    void setServiceSearchConfig(const QStringList &paths);
private:
    ConfigManager() = default;
    ~ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    QString m_filePath;
    QJsonObject m_configJson;

    void createDefaultConfig();
};

#endif