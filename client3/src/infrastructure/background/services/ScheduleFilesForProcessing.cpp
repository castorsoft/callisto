#include "ScheduleFilesForProcessing.h"

#include <QFileInfo>
#include "../../database/repositories/ArquivoRepository.h"

void ScheduleFilesForProcessing::startService()
{
    if (m_isRunning) return;

    emit serviceStarted();
}

ScheduleFilesForProcessing::ScheduleFilesForProcessing(QObject *parent): IService(parent)
{
    m_arquivoRepository = new ArquivoRepository();
}

ScheduleFilesForProcessing::~ScheduleFilesForProcessing()
{
    stopService();
    delete m_arquivoRepository;
}

bool ScheduleFilesForProcessing::isRunning() const
{
    return m_isRunning;
}

void ScheduleFilesForProcessing::stopService()
{
    if (!m_isRunning) return;
    IService::stopService();

    emit serviceStopped();
}

void ScheduleFilesForProcessing::onFileAdded(const QString &filePath)
{
    QFileInfo arquivoInfo = QFileInfo(filePath);

    if (!m_arquivoRepository->existeArquivo(filePath)) {
        Arquivo modelArquivo(arquivoInfo.absoluteFilePath(), arquivoInfo.fileName(), false);

        m_arquivoRepository->salvar(modelArquivo);
        emit requestFileProcess(modelArquivo);
    }
}