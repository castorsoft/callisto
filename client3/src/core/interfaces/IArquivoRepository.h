#pragma once

#include "../entities/Arquivo.h"

#include <QString>
#include <QList>

class IArquivoRepository {
public:
    virtual ~IArquivoRepository() = default;
    virtual bool salvar(Arquivo &modelArquivo) = 0;
    virtual bool existeArquivo(const QString &filePath, Arquivo *modelArquivo = nullptr) = 0;
    virtual QList<Arquivo> carregarArquivosPendentes() = 0;
    virtual std::optional<Arquivo> buscarPorUuid(const QUuid &uuid) = 0;
};