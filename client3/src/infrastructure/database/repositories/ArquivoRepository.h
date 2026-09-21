#pragma once

#include <QSqlDatabase>

#include "../../../core/interfaces/IArquivoRepository.h"
#include "../../../core/entities/Arquivo.h"


class ArquivoRepository: public IArquivoRepository {
public:
    ArquivoRepository();

    void salvar(Arquivo &modelArquivo);
    bool existeArquivo(const QString &filePath, Arquivo *modelArquivo = nullptr) override;
    QList<Arquivo> carregarArquivosPendentes() override;
    std::optional<Arquivo> buscarPorUuid(const QUuid &uuid) override;
private:
    QSqlDatabase m_db;
};