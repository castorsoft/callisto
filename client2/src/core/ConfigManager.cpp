#include "ConfigManager.h"

#include <QFile>
#include <QByteArray>
#include <QJsonParseError>
#include <QJsonArray>

void ConfigManager::createDefaultConfig()
{
    QJsonObject dbConfig;
    dbConfig["host"] = "localhost";
    dbConfig["port"] = 5432;
    dbConfig["database"] = "callisto";
    dbConfig["user"] = "postgres";
    dbConfig["password"] = "secret";

    QJsonArray pathsItemsConfig;
    pathsItemsConfig.push_back("./");

    QJsonObject pathsConfig;
    pathsConfig["paths"] = pathsItemsConfig;

    QJsonObject servicesConfig;
    servicesConfig["search"] = pathsConfig;

    // QJsonObject appConfig;
    // appConfig["theme"] = "system";

    m_configJson["database"] = dbConfig;
    // m_configJson["application"] = appConfig;
    m_configJson["services"] = servicesConfig;
}

ConfigManager &ConfigManager::instance()
{
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::load(const QString &filePath)
{
    m_filePath = filePath;
    QFile file(m_filePath);
    if (!file.exists()) {
        qInfo() << "[ConfigManager] Arquivo de configuracao nao encontrado. Criando padrao em:" << m_filePath;
        createDefaultConfig();
        return save();
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "[ConfigManager] Nao foi possivel abrir o arquivo" << m_filePath << "para leitura.";
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qCritical() << "[ConfigManager] Erro ao interpretar JSON: " << parseError.errorString();
        return false;
    }

    if (!doc.isObject()) {
        qCritical() << "[ConfigManager] O arquivo JSON nao contem um objeto valido.";
        return false;
    }

    m_configJson = doc.object();
    qInfo() << "[ConfigManager] Configuracoes carregadas de:" << m_filePath;

    return true;
}

bool ConfigManager::save() const
{
    QFile file(m_filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "[ConfigManager] Nao foi possivel abrir o arquivo" << m_filePath << "para escrita.";
        return false;
    }

    QJsonDocument doc(m_configJson);

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

QString ConfigManager::dbHost() const
{
    return m_configJson["database"].toObject()["host"].toString("localhost");
}

int ConfigManager::dbPort() const
{
    return m_configJson["database"].toObject()["port"].toInt(5432);
}

QString ConfigManager::dbName() const
{
    return m_configJson["database"].toObject()["database"].toString("callisto");
}

QString ConfigManager::dbUser() const
{
    return m_configJson["database"].toObject()["user"].toString("postgres");
}

QString ConfigManager::dbPassword() const
{
    return m_configJson["database"].toObject()["password"].toString("");
}

QStringList ConfigManager::searchPaths() const
{
    QStringList list(m_configJson["services"].toObject()["search"].toObject()["paths"].toVariant().toStringList());
    return list;
}

void ConfigManager::setServiceSearchConfig(const QStringList &paths)
{
    QJsonArray pathsItemsConfig;
    for (const auto &path: paths) {
        pathsItemsConfig.push_back(path);
    }

    QJsonObject pathsConfig;
    pathsConfig["paths"] = pathsItemsConfig;

    m_configJson["services"].toObject().insert("search", pathsConfig);
}

void ConfigManager::setDatabaseConfig(const QString &host, int port, const QString &dbName, const QString &user, const QString &password)
{
    QJsonObject dbConfig;

    dbConfig["host"] = host;
    dbConfig["port"] = port;
    dbConfig["database"] = dbName;
    dbConfig["user"] = user;
    dbConfig["password"] = password;

    m_configJson["database"] = dbConfig;
}
