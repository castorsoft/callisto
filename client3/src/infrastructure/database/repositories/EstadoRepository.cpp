#include "EstadoRepository.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>

EstadoRepository::EstadoRepository() {
    m_db = QSqlDatabase::database();
}

QList<Estado> EstadoRepository::listarTodos() {
    QList<Estado> estados;
    QSqlQuery query(m_db);
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
