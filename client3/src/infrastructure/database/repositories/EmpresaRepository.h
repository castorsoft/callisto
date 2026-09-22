#pragma once

#include <QSqlDatabase>
#include "../../../core/interfaces/IEmpresaRepository.h"
#include "../../../core/entities/Empresa.h"

class EmpresaRepository : public IEmpresaRepository {
public:
    EmpresaRepository();

    void salvar(Empresa &modelEmpresa) override;
    std::optional<Empresa> buscarPorId(int id) override;
    std::optional<Empresa> buscarPorUuid(const QUuid &uuid) override;
    std::optional<Empresa> buscarPorCnpjCpf(const QString &cnpjCpf) override;
    QList<Empresa> listarTodas() override;
    bool deletar(int id) override;

private:
    QSqlDatabase m_db;

    QList<int> carregarContatos(int empresaId);
    void salvarContatos(int empresaId, const QList<int> &contatosIds);
    void limparContatos(int empresaId);

    std::optional<Empresa> mapToEntity(class QSqlQuery &query);
};
