#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <memory>

template <typename T>
class Model {
public:
    virtual ~Model() = default;
    virtual QSqlDatabase database() const { return QSqlDatabase::database(); }

    static std::unique_ptr<T> find(int a_id) {
        T tempInstance; // Usada para pegar o nome da tabela
        QSqlQuery query;
        query.prepare(QString("SELECT * FROM %1 WHERE id = :id").arg(tempInstance.tableName()));
        query.bindValue(":id", a_id);

        if (query.exec()) {
            if (query.size() > 0) {
                T* instance = std::make_unique<T>();
                instance->fill(query.record());
                return instance;
            } else {
                return nullptr;
            }
        } else {
            throw DatabaseException(query.lastError().text());
        }
    }

    static std::unique_ptr<T> findByField(QString a_fieldName, QVariant a_value) {
        T tempInstance; // Usada para pegar o nome da tabela
        QSqlQuery query;
        query.prepare(QString("SELECT * FROM %1 WHERE %2 = :value").arg(tempInstance.tableName(), a_fieldName));
        query.bindValue(":value", a_value);

        if (query.exec()) {
            if (query.size() > 0) {
                auto instance = std::make_unique<T>();
                instance->fill(query.record());
                return instance;
            } else {
                return nullptr;
            }
        } else {
            throw DatabaseException(query.lastError().text());
        }

    }

    // Métodos que as subclasses devem implementar
    virtual QString tableName() const = 0;
    virtual void fill(const QSqlRecord &record) = 0;
};