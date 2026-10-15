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

    bool salvar(NCM &ncm);
    std::optional<NCM> buscarPorCodigo(const QString &codigo);
    QList<NCM> buscarPorTermo(const QString &termo) override;
};
