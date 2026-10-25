#include "SpedFileWriter.h"
#include "../records/bloco9/RegistroBloco9.h"
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QFileInfo>
#include <QDir>

SpedWriteResult SpedFileWriter::writeToFile(
    std::vector<std::shared_ptr<SpedRecordBase>> records,
    const QString &caminhoArquivo)
{
    SpedWriteResult result;
    result.caminhoArquivo = caminhoArquivo;

    // Garante que o diretório de destino existe
    QFileInfo fi(caminhoArquivo);
    QDir dir = fi.dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // ==========================================
    // CÁLCULO E CONSTRUÇÃO DO BLOCO 9 (TOTALIZADORES)
    // ==========================================
    QMap<QString, int> regCount;
    for (const auto &r : records) {
        regCount[r->registerName()]++;
    }

    // Bloco 9 terá 9001, 9990, 9999 e os 9900
    QSet<QString> todosTipos = QSet<QString>(regCount.keyBegin(), regCount.keyEnd());
    todosTipos.insert("9001");
    todosTipos.insert("9900");
    todosTipos.insert("9990");
    todosTipos.insert("9999");

    QStringList tiposOrdenados = todosTipos.values();
    tiposOrdenados.sort();

    int qtdLin9900 = tiposOrdenados.size();
    regCount["9001"] = 1;
    regCount["9900"] = qtdLin9900;
    regCount["9990"] = 1;
    regCount["9999"] = 1;

    // 9001: Abertura do Bloco 9
    records.push_back(std::make_shared<Registro9001>(0));

    // 9900: Totalizadores por tipo de registro
    for (const QString &tipo : tiposOrdenados) {
        records.push_back(std::make_shared<Registro9900>(tipo, regCount.value(tipo, 0)));
    }

    // 9990: Encerramento do Bloco 9
    int qtdLinhasBloco9 = 1 + qtdLin9900 + 1; // 9001 + 9900s + 9990
    records.push_back(std::make_shared<Registro9990>(qtdLinhasBloco9));

    // 9999: Encerramento do Arquivo Digital (total geral de linhas)
    int totalGeralLinhas = records.size() + 1; // inclusive o próprio 9999
    records.push_back(std::make_shared<Registro9999>(totalGeralLinhas));

    // ==========================================
    // GRAVAÇÃO NO ARQUIVO .TXT
    // ==========================================
    QFile file(caminhoArquivo);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        result.sucesso = false;
        result.mensagem = "Falha ao criar o arquivo no caminho: " + file.errorString();
        return result;
    }

    QTextStream out(&file);
    // Codificação padrão do SPED Fiscal: ISO-8859-1 (Latin1) ou UTF-8
    out.setEncoding(QStringConverter::Latin1);

    for (const auto &r : records) {
        out << r->buildLine() << "\r\n";
    }

    out.flush();
    result.bytesEscritos = file.size();
    file.close();

    result.sucesso = true;
    result.totalLinhas = records.size();
    result.mensagem = QString("SPED Fiscal gerado com sucesso! Total de %1 linhas.").arg(result.totalLinhas);

    return result;
}

