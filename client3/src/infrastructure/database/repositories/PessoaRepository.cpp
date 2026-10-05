#include "PessoaRepository.h"
#include "EnderecoRepository.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include "../DatabaseManager.h"
#include <stdexcept>
#include <QSqlRecord>

PessoaRepository::PessoaRepository() {
    m_db = DatabaseManager::instance().database();
}

std::optional<Pessoa> PessoaRepository::mapToEntity(QSqlQuery &query) {
    auto pessoa = Pessoa::fromRecord(query.record());

    EnderecoRepository endRepo;
    auto endOpt = endRepo.buscarPorId(pessoa.idEndereco());
    if (endOpt.has_value()) {
        pessoa.setEndereco(endOpt.value());
    }

    return pessoa;
}

void PessoaRepository::salvar(Pessoa &modelPessoa) {
    Endereco end = modelPessoa.endereco();
    EnderecoRepository endRepo;
    endRepo.salvar(end);
    modelPessoa.setEndereco(end);

    QSqlQuery query(m_db);

    if (!modelPessoa.id().has_value() || modelPessoa.id().value() == -1) {
        query.prepare("INSERT INTO pessoas (id_empresa, cnpj_cpf, razao_social, nome_fantasia, inscricao_estadual, fornecedor, contabilista, id_endereco) "
                      "VALUES (:id_empresa, :cnpj_cpf, :razao_social, :nome_fantasia, :inscricao_estadual, :fornecedor, :contabilista, :id_endereco) "
                      "RETURNING id, uuid, criado_em, atualizado_em");
    } else {
        query.prepare("UPDATE pessoas SET id_empresa = :id_empresa, cnpj_cpf = :cnpj_cpf, razao_social = :razao_social, nome_fantasia = :nome_fantasia, "
                      "inscricao_estadual = :inscricao_estadual, fornecedor = :fornecedor, contabilista = :contabilista, id_endereco = :id_endereco, atualizado_em = CURRENT_TIMESTAMP "
                      "WHERE id = :id RETURNING atualizado_em");
        query.bindValue(":id", modelPessoa.id().value());
    }

    query.bindValue(":id_empresa", modelPessoa.idEmpresa());
    query.bindValue(":cnpj_cpf", modelPessoa.cnpjCpf());
    query.bindValue(":razao_social", modelPessoa.razaoSocial());
    query.bindValue(":nome_fantasia", modelPessoa.nomeFantasia().has_value() ? modelPessoa.nomeFantasia().value() : QVariant());
    query.bindValue(":inscricao_estadual", modelPessoa.inscricaoEstadual().has_value() ? modelPessoa.inscricaoEstadual().value() : QVariant());
    query.bindValue(":fornecedor", modelPessoa.fornecedor());
    query.bindValue(":contabilista", modelPessoa.contabilista());
    query.bindValue(":id_endereco", modelPessoa.idEndereco());

    if (query.exec() && query.next()) {
        if (!modelPessoa.id().has_value() || modelPessoa.id().value() == -1) {
            modelPessoa.setId(query.value("id").toInt());
            modelPessoa.setUuid(query.value("uuid").toUuid());
            modelPessoa.setCriadoEm(query.value("criado_em").toDateTime());
            modelPessoa.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        } else {
            modelPessoa.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        }
    } else {
        throw std::runtime_error("Erro ao salvar pessoa no banco: " + query.lastError().text().toStdString());
    }
}

std::optional<Pessoa> PessoaRepository::buscarPorId(int id) {
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM pessoas WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) return mapToEntity(query);
    return std::nullopt;
}

std::optional<Pessoa> PessoaRepository::buscarPorUuid(const QUuid &uuid) {
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM pessoas WHERE uuid = :uuid");
    query.bindValue(":uuid", uuid);

    if (query.exec() && query.next()) return mapToEntity(query);
    return std::nullopt;
}

std::optional<Pessoa> PessoaRepository::buscarPorCnpjCpf(const QString &cnpjCpf) {
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM pessoas WHERE cnpj_cpf = :cnpj_cpf");
    query.bindValue(":cnpj_cpf", cnpjCpf);

    if (query.exec() && query.next()) return mapToEntity(query);
    return std::nullopt;
}

QList<Pessoa> PessoaRepository::listarTodas() {
    QList<Pessoa> lista;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM pessoas ORDER BY razao_social");

    if (query.exec()) {
        while (query.next()) {
            if (auto p = mapToEntity(query)) lista.append(p.value());
        }
    }
    return lista;
}

bool PessoaRepository::deletar(int id) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM pessoas WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}
