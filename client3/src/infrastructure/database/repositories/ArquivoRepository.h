#pragma once

#include <QSqlDatabase>

#include "../../../core/interfaces/IArquivoRepository.h"
#include "../../../core/entities/Arquivo.h"


class ArquivoRepository: public IArquivoRepository {
public:
    ArquivoRepository();

    void salvar(Arquivo &modelArquivo);
    bool existeArquivo(const QString &filePath, Arquivo *modelArquivo);
    QList<Arquivo> carregarArquivosPendentes();
private:
    QSqlDatabase m_db;
};