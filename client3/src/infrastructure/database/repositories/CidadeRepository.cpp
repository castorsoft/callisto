#include "CidadeRepository.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>

CidadeRepository::CidadeRepository() {
    m_db = QSqlDatabase::database();
}

QList<Cidade> CidadeRepository::listarPorEstado(int idEstado) {
    QList<Cidade> cidades;
    QSqlQuery query(m_db);
    query.prepare("SELECT id, nome, id_ibge, id_estado FROM cidades WHERE id_estado = :id_estado ORDER BY nome");
    query.bindValue(":id_estado", idEstado);
    
    if (query.exec()) {
        while (query.next()) {
            cidades.append(Cidade(
                query.value("id").toInt(),
                query.value("nome").toString(),
                query.value("id_ibge").toInt(),
                query.value("id_estado").toInt()
            ));
        }
    }
    return cidades;
}
