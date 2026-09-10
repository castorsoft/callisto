#pragma once

#include "../entities/Arquivo.h"

#include <QString>
#include <QList>

class IArquivoRepository {
public:
    virtual ~IArquivoRepository() = default;
    virtual void salvar(Arquivo &modelArquivo) = 0;
    virtual bool existeArquivo(const QString &filePath, Arquivo *modelArquivo) = 0;
    virtual QList<Arquivo> carregarArquivosPendentes() = 0;
};