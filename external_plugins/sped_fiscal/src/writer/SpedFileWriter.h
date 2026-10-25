#pragma once

#include <QString>
#include <vector>
#include <memory>
#include "../records/SpedRecordBase.h"

struct SpedWriteResult {
    bool sucesso{false};
    QString mensagem;
    int totalLinhas{0};
    qint64 bytesEscritos{0};
    QString caminhoArquivo;
};

class SpedFileWriter {
public:
    static SpedWriteResult writeToFile(
        std::vector<std::shared_ptr<SpedRecordBase>> records,
        const QString &caminhoArquivo
    );
};

