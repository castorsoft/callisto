#include "ScheduleFilesForProcessing.h"

#include <QFileInfo>
#include <QThread>
#include <QDebug>
#include <algorithm>
#include "../../database/repositories/ArquivoRepository.h"

ScheduleFilesForProcessing::ScheduleFilesForProcessing(QObject *parent): IService(parent)
{
}

ScheduleFilesForProcessing::~ScheduleFilesForProcessing()
{
    stopService();
}

void ScheduleFilesForProcessing::startService()
{
    if (m_isRunning) return;

    m_preProcessPool.setMaxThreadCount(std::max(2, QThread::idealThreadCount() / 2));
    m_isRunning = true;
    emit serviceStarted();

    qInfo() << "[ScheduleFilesForProcessing] Pool de pré-processamento iniciado com"
            << m_preProcessPool.maxThreadCount() << "threads.";
}

bool ScheduleFilesForProcessing::isRunning() const
{
    return m_isRunning;
}

void ScheduleFilesForProcessing::stopService()
{
    if (!m_isRunning) return;
    IService::stopService();

    m_isRunning = false;
    m_preProcessPool.clear();
    m_preProcessPool.waitForDone(5000);

    emit serviceStopped();
    qInfo() << "[ScheduleFilesForProcessing] Pool de pré-processamento encerrado.";
}

void ScheduleFilesForProcessing::onFileAdded(const QString &filePath)
{
    if (!m_isRunning) {
        qWarning() << "[ScheduleFilesForProcessing] Servico parado, ignorando arquivo:" << filePath;
        return;
    }

    m_preProcessPool.start([this, filePath]() {
        preProcessFile(filePath);
    });
}

void ScheduleFilesForProcessing::preProcessFile(const QString &filePath)
{
    QFileInfo arquivoInfo(filePath);
    if (!arquivoInfo.exists()) {
        qWarning() << "[ScheduleFilesForProcessing] Arquivo nao encontrado:" << filePath;
        return;
    }

    // Caso o arquivo ainda esteja sendo gravado pelo SO, aguarda brevemente
    int attempts = 0;
    while (arquivoInfo.size() == 0 && attempts < 5) {
        QThread::msleep(100);
        arquivoInfo.refresh();
        attempts++;
    }

    QString caminhoAbsoluto = arquivoInfo.absoluteFilePath();
    ArquivoRepository repo;

    if (repo.existeArquivo(caminhoAbsoluto)) {
        qInfo() << "[ScheduleFilesForProcessing] Arquivo ja registrado no banco:" << caminhoAbsoluto;
        return;
    }

    Arquivo modelArquivo(caminhoAbsoluto, arquivoInfo.fileName(), false);
    repo.salvar(modelArquivo);

    if (modelArquivo.uuid().has_value()) {
        qInfo() << "[ScheduleFilesForProcessing] Arquivo pré-processado com sucesso. UUID:"
                << modelArquivo.uuid().value() << "Arquivo:" << arquivoInfo.fileName();
        emit requestFileProcess(modelArquivo.uuid().value());
    } else {
        qWarning() << "[ScheduleFilesForProcessing] Falha ao obter UUID após salvar o arquivo:" << caminhoAbsoluto;
    }
}