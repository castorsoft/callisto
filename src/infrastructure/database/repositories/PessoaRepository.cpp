#include "PessoaRepository.h"
#include "EnderecoRepository.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include "../DatabaseManager.h"
#include <stdexcept>
#include <QSqlRecord>

PessoaRepository::PessoaRepository() {
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

bool PessoaRepository::salvar(Pessoa &modelPessoa) {
    Endereco end = modelPessoa.endereco();
    EnderecoRepository endRepo;
    endRepo.salvar(end);
    modelPessoa.setEndereco(end);

    QSqlQuery query(DatabaseManager::instance().database());

    if (!modelPessoa.id().has_value() || modelPessoa.id().value() == -1) {
        query.prepare(  "INSERT INTO pessoas (id_empresa, cnpj_cpf, razao_social, nome_fantasia, inscricao_estadual, fornecedor, contabilista, id_endereco) "
                        "VALUES (:id_empresa, :cnpj_cpf, :razao_social, :nome_fantasia, :inscricao_estadual, :fornecedor, :contabilista, :id_endereco) "
                        "RETURNING id, uuid, criado_em, atualizado_em");
    } else {
        query.prepare(  "UPDATE pessoas SET "
                        "id_empresa = :id_empresa, "
                        "cnpj_cpf = :cnpj_cpf, "
                        "razao_social = :razao_social, "
                        "nome_fantasia = :nome_fantasia, "
                        "inscricao_estadual = :inscricao_estadual, "
                        "fornecedor = :fornecedor, "
                        "contabilista = :contabilista, "
                        "id_endereco = :id_endereco "
                        "WHERE id = :id "
                        "RETURNING atualizado_em");

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
        modelPessoa.setAtualizadoEm(query.value("atualizado_em").toDateTime());
        if (!modelPessoa.id().has_value()) {
            modelPessoa.setId(query.value("id").toInt());
            modelPessoa.setUuid(query.value("uuid").toUuid());
            modelPessoa.setCriadoEm(query.value("criado_em").toDateTime());
        }

        int idPessoa = modelPessoa.id().value();
        if (modelPessoa.contabilista() == "S" && modelPessoa.pessoaContabilista().has_value()) {
            PessoaContabilista pc = modelPessoa.pessoaContabilista().value();
            pc.setIdPessoa(idPessoa);

            QSqlQuery checkQuery(DatabaseManager::instance().database());
            checkQuery.prepare("SELECT id FROM public.pessoas_contabilista WHERE id_pessoa = :id_pessoa LIMIT 1");
            checkQuery.bindValue(":id_pessoa", idPessoa);

            QSqlQuery pcQuery(DatabaseManager::instance().database());
            if (checkQuery.exec() && checkQuery.next()) {
                int idPc = checkQuery.value("id").toInt();
                pc.setId(idPc);
                pcQuery.prepare(
                    "UPDATE public.pessoas_contabilista SET "
                    "cpf = :cpf, "
                    "crc = :crc "
                    "WHERE id = :id "
                    "RETURNING atualizado_em"
                );
                pcQuery.bindValue(":id", idPc);
            } else {
                pcQuery.prepare(
                    "INSERT INTO public.pessoas_contabilista (id_pessoa, cpf, crc) "
                    "VALUES (:id_pessoa, :cpf, :crc) "
                    "RETURNING id, criado_em, atualizado_em"
                );
                pcQuery.bindValue(":id_pessoa", idPessoa);
            }

            pcQuery.bindValue(":cpf", pc.cpf().isEmpty() ? QVariant() : pc.cpf());
            pcQuery.bindValue(":crc", pc.crc());

            if (pcQuery.exec() && pcQuery.next()) {
                if (!pc.id().has_value()) {
                    pc.setId(pcQuery.value("id").toInt());
                    pc.setCriadoEm(pcQuery.value("criado_em").toDateTime());
                }
                pc.setAtualizadoEm(pcQuery.value("atualizado_em").toDateTime());
                modelPessoa.setPessoaContabilista(pc);
            } else {
                qCritical() << "[PessoaRepository] Erro ao salvar dados de contabilista:" << pcQuery.lastError().text();
            }
        } else {
            QSqlQuery delQuery(DatabaseManager::instance().database());
            delQuery.prepare("DELETE FROM public.pessoas_contabilista WHERE id_pessoa = :id_pessoa");
            delQuery.bindValue(":id_pessoa", idPessoa);
            delQuery.exec();
            modelPessoa.setPessoaContabilista(std::nullopt);
        }

        return true;
    } else {
        qCritical() << "[PessoaRepository] Erro ao inserir/atualizar pessoa:" << query.lastError().text();

        return false;
    }
}

std::optional<Pessoa> PessoaRepository::buscarPorId(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT p.*, "
        "pc.id AS cont_id, pc.id_pessoa AS cont_id_pessoa, pc.cpf AS cont_cpf, pc.crc AS cont_crc, "
        "pc.criado_em AS cont_criado_em, pc.atualizado_em AS cont_atualizado_em "
        "FROM public.pessoas p "
        "LEFT JOIN public.pessoas_contabilista pc ON pc.id_pessoa = p.id "
        "WHERE p.id = :id"
    );
    query.bindValue(":id", id);

    if (query.exec() && query.next()) return mapToEntity(query);
    return std::nullopt;
}

std::optional<Pessoa> PessoaRepository::buscarPorUuid(const QUuid &uuid) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT p.*, "
        "pc.id AS cont_id, pc.id_pessoa AS cont_id_pessoa, pc.cpf AS cont_cpf, pc.crc AS cont_crc, "
        "pc.criado_em AS cont_criado_em, pc.atualizado_em AS cont_atualizado_em "
        "FROM public.pessoas p "
        "LEFT JOIN public.pessoas_contabilista pc ON pc.id_pessoa = p.id "
        "WHERE p.uuid = :uuid"
    );
    query.bindValue(":uuid", uuid);

    if (query.exec() && query.next()) return mapToEntity(query);
    return std::nullopt;
}

std::optional<Pessoa> PessoaRepository::buscarPorCnpjCpf(const QString &cnpjCpf) {
    auto cnpjCpfTratado = cnpjCpf;
    cnpjCpfTratado.remove('.').remove('-').remove('/');

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT p.*, "
        "pc.id AS cont_id, pc.id_pessoa AS cont_id_pessoa, pc.cpf AS cont_cpf, pc.crc AS cont_crc, "
        "pc.criado_em AS cont_criado_em, pc.atualizado_em AS cont_atualizado_em "
        "FROM public.pessoas p "
        "LEFT JOIN public.pessoas_contabilista pc ON pc.id_pessoa = p.id "
        "WHERE p.cnpj_cpf = :cnpj_cpf"
    );
    query.bindValue(":cnpj_cpf", cnpjCpfTratado);

    if (query.exec() && query.next()) return mapToEntity(query);
    return std::nullopt;
}

QList<Pessoa> PessoaRepository::buscarContadorPorTermo(const QString &termo) {
    QList<Pessoa> lista;

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT p.*, "
        "pc.id AS cont_id, pc.id_pessoa AS cont_id_pessoa, pc.cpf AS cont_cpf, pc.crc AS cont_crc, "
        "pc.criado_em AS cont_criado_em, pc.atualizado_em AS cont_atualizado_em "
        "FROM public.pessoas p "
        "LEFT JOIN public.pessoas_contabilista pc ON pc.id_pessoa = p.id "
        "WHERE p.contabilista = 'S' "
        "AND (p.razao_social ILIKE :termo OR p.nome_fantasia ILIKE :termo OR p.cnpj_cpf ILIKE :termo) "
        "ORDER BY p.razao_social"
    );
    query.bindValue(":termo", "%" + termo + "%");

    if (query.exec()) {
        while (query.next()) {
            lista.append(Pessoa::fromRecord(query.record()));
        }
    } else {
        qCritical() << "[PessoaRepository] Erro ao buscar contadores por termo:" << query.lastError().text();
    }
    return lista;
}

QList<Pessoa> PessoaRepository::listarTodas() {
    QList<Pessoa> lista;

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT p.*, "
        "pc.id AS cont_id, pc.id_pessoa AS cont_id_pessoa, pc.cpf AS cont_cpf, pc.crc AS cont_crc, "
        "pc.criado_em AS cont_criado_em, pc.atualizado_em AS cont_atualizado_em "
        "FROM public.pessoas p "
        "LEFT JOIN public.pessoas_contabilista pc ON pc.id_pessoa = p.id "
        "ORDER BY p.razao_social"
    );

    if (query.exec()) {
        while (query.next()) {
            lista.append(Pessoa::fromRecord(query.record()));
        }
    } else {
        qCritical() << "[PessoaRepository] Erro ao listar pessoas:" << query.lastError().text();
    }
    return lista;
}

bool PessoaRepository::deletar(int id) {
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM pessoas WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}
