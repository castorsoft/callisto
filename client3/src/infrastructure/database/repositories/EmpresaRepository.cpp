#include "EmpresaRepository.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include "../DatabaseManager.h"

EmpresaRepository::EmpresaRepository()
{
    m_db = DatabaseManager::instance().database();
}

QList<int> EmpresaRepository::carregarContatos(int empresaId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id_contato FROM empresa_contatos WHERE id_empresa = :id_empresa");
    query.bindValue(":id_empresa", empresaId);

    QList<int> contatos;
    if (query.exec()) {
        while (query.next()) {
            contatos.append(query.value(0).toInt());
        }
    }
    return contatos;
}

void EmpresaRepository::limparContatos(int empresaId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM empresa_contatos WHERE id_empresa = :id_empresa");
    query.bindValue(":id_empresa", empresaId);
    query.exec();
}

void EmpresaRepository::salvarContatos(int empresaId, const QList<int> &contatosIds)
{
    limparContatos(empresaId);

    if (contatosIds.isEmpty()) return;

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO empresa_contatos (id_empresa, id_contato) VALUES (:id_empresa, :id_contato)");

    QVariantList empresas;
    QVariantList contatos;
    for (int idContato : contatosIds) {
        empresas << empresaId;
        contatos << idContato;
    }

    query.bindValue(":id_empresa", empresas);
    query.bindValue(":id_contato", contatos);

    query.execBatch();
}

std::optional<Empresa> EmpresaRepository::mapToEntity(QSqlQuery &query)
{
    int id = query.value("id").toInt();
    QList<int> contatos = carregarContatos(id);

    return Empresa::loadFromDatabase(
        id,
        query.value("uuid").toUuid(),
        query.value("cnpj_cpf").toString(),
        query.value("razao_social").toString(),
        query.value("nome_fantasia").isNull() ? std::nullopt : std::make_optional(query.value("nome_fantasia").toString()),
        query.value("regime_tributario").toString(),
        query.value("id_endereco").toInt(),
        query.value("inscricao_estadual").isNull() ? std::nullopt : std::make_optional(query.value("inscricao_estadual").toString()),
        query.value("inscricao_municipal").isNull() ? std::nullopt : std::make_optional(query.value("inscricao_municipal").toString()),
        contatos,
        query.value("criado_em").toDateTime(),
        query.value("atualizado_em").toDateTime()
    );
}

void EmpresaRepository::salvar(Empresa &modelEmpresa)
{
    QSqlQuery query(m_db);

    if (!modelEmpresa.id().has_value() || modelEmpresa.id().value() == -1) {
        query.prepare("INSERT INTO empresas (cnpj_cpf, razao_social, nome_fantasia, regime_tributario, id_endereco, inscricao_estadual, inscricao_municipal) "
                      "VALUES (:cnpj_cpf, :razao_social, :nome_fantasia, :regime_tributario, :id_endereco, :inscricao_estadual, :inscricao_municipal) "
                      "RETURNING id, uuid, criado_em, atualizado_em");
    } else {
        query.prepare("UPDATE empresas SET cnpj_cpf = :cnpj_cpf, razao_social = :razao_social, nome_fantasia = :nome_fantasia, "
                      "regime_tributario = :regime_tributario, id_endereco = :id_endereco, inscricao_estadual = :inscricao_estadual, "
                      "inscricao_municipal = :inscricao_municipal, atualizado_em = CURRENT_TIMESTAMP "
                      "WHERE id = :id "
                      "RETURNING atualizado_em");
        query.bindValue(":id", modelEmpresa.id().value());
    }

    query.bindValue(":cnpj_cpf", modelEmpresa.cnpjCpf());
    query.bindValue(":razao_social", modelEmpresa.razaoSocial());
    query.bindValue(":nome_fantasia", modelEmpresa.nomeFantasia().has_value() ? modelEmpresa.nomeFantasia().value() : QVariant());
    query.bindValue(":regime_tributario", modelEmpresa.regimeTributario());
    query.bindValue(":id_endereco", modelEmpresa.idEndereco());
    query.bindValue(":inscricao_estadual", modelEmpresa.inscricaoEstadual().has_value() ? modelEmpresa.inscricaoEstadual().value() : QVariant());
    query.bindValue(":inscricao_municipal", modelEmpresa.inscricaoMunicipal().has_value() ? modelEmpresa.inscricaoMunicipal().value() : QVariant());

    if (query.exec() && query.next()) {
        if (!modelEmpresa.id().has_value() || modelEmpresa.id().value() == -1) {
            modelEmpresa.hidratar(query.value("id").toInt(), query.value("uuid").toUuid(), query.value("criado_em").toDateTime(), query.value("atualizado_em").toDateTime());
        } else {
            modelEmpresa.hidratar(query.value("atualizado_em").toDateTime());
        }

        salvarContatos(modelEmpresa.id().value(), modelEmpresa.contatosIds());
    }
}

std::optional<Empresa> EmpresaRepository::buscarPorId(int id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM empresas WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return mapToEntity(query);
    }
    return std::nullopt;
}

std::optional<Empresa> EmpresaRepository::buscarPorUuid(const QUuid &uuid)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM empresas WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);

    if (query.exec() && query.next()) {
        return mapToEntity(query);
    }
    return std::nullopt;
}

std::optional<Empresa> EmpresaRepository::buscarPorCnpjCpf(const QString &cnpjCpf)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM empresas WHERE cnpj_cpf = :cnpj_cpf");
    query.bindValue(":cnpj_cpf", cnpjCpf);

    if (query.exec() && query.next()) {
        return mapToEntity(query);
    }
    return std::nullopt;
}

QList<Empresa> EmpresaRepository::listarTodas()
{
    QList<Empresa> lista;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM empresas ORDER BY razao_social");

    if (query.exec()) {
        while (query.next()) {
            if (auto empresa = mapToEntity(query)) {
                lista.append(empresa.value());
            }
        }
    }
    return lista;
}

bool EmpresaRepository::deletar(int id)
{
    limparContatos(id);
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM empresas WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}
