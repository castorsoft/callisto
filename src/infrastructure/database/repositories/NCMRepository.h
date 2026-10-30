#pragma once

#include <optional>
#include <QUuid>
#include <QList>
#include "../../../core/interfaces/INCMRepository.h"
#include "../../../core/entities/NCM.h"

class NCMRepository : public INCMRepository {
public:
    NCMRepository() = default;
    ~NCMRepository() override = default;

    bool salvar(NCM &ncm) override;
    std::optional<NCM> buscarPorId(int id) override;
    std::optional<NCM> buscarPorCodigo(const QString &codigo) override;
    QList<NCM> buscarPorTermo(const QString &termo) override;
};
