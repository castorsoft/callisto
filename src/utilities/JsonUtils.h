#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <type_traits>

class JsonUtils {
public:
    // Retorna QJsonValue encontrado ou undefined se não existir
    static QJsonValue getValue(const QJsonObject &root, const QString &path) {
        QStringList parts = path.split('.');
        QJsonObject curObj = root;
        for (int i = 0; i < parts.size(); ++i) {
            const QString &key = parts.at(i);
            QJsonValue v = curObj.value(key);
            if (i == parts.size() - 1) return v;
            if (!v.isObject()) return QJsonValue(); // não encontrou caminho
            curObj = v.toObject();
        }
        return QJsonValue();
    }

    // Template para recuperar valor convertido; fornece defaultValue se não existir ou conversão falhar
    template<typename T>
    static T getValue(const QJsonObject &root, const QString &path, const T &defaultValue = T()) {
        QJsonValue v = getValue(root, path);
        if (v.isUndefined() || v.isNull()) return defaultValue;

        if constexpr (std::is_same_v<T, QString>) {
            return v.toString();
        } else if constexpr (std::is_same_v<T, int>) {
            return v.toInt(static_cast<int>(defaultValue));
        } else if constexpr (std::is_same_v<T, double>) {
            return v.toDouble(static_cast<double>(defaultValue));
        } else if constexpr (std::is_same_v<T, bool>) {
            return v.toBool();
        } else if constexpr (std::is_same_v<T, QJsonObject>) {
            return v.toObject();
        } else if constexpr (std::is_same_v<T, QJsonArray>) {
            return v.toArray();
        } else {
            QVariant var = v.toVariant();
            if (!var.isValid() || var.isNull()) return defaultValue;
            // tenta qvariant_cast para tipos registrados no meta-type system
            if (var.canConvert<T>()) {
                return var.value<T>();
            }
            return qvariant_cast<T>(var);
        }
    }
};

#endif