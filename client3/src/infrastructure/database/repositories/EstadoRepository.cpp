#include "EstadoRepository.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include "../DatabaseManager.h"

EstadoRepository::EstadoRepository() {
}

QList<Estado> EstadoRepository::listarTodos() {
    QList<Estado> estados;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT id, nome, sigla, id_ibge FROM estados ORDER BY sigla");

    if (query.exec()) {
        while (query.next()) {
            estados.append(Estado(
                query.value("id").toInt(),
                query.value("nome").toString(),
                query.value("sigla").toString(),
                query.value("id_ibge").toInt()
            ));
        }
    }
    return estados;
}
