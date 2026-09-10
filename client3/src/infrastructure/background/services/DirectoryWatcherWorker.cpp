#include "DirectoryWatcherWorker.h"

#include "../../../core/entities/Arquivo.h"
#include "../../database/repositories/ArquivoRepository.h"

#include <QDir>
#include <QThread>
#include <QDebug>
#include <QMutexLocker>

DirectoryWatcherWorker::DirectoryWatcherWorker(const QString &watchPath, QObject *parent)
    : IService(parent), m_watchPath(watchPath)
{
    m_arquivoRepository = new ArquivoRepository();
}

DirectoryWatcherWorker::~DirectoryWatcherWorker()
{
    stopService();
}

bool DirectoryWatcherWorker::isRunning() const
{
    return m_isRunning;
}

void DirectoryWatcherWorker::updateKnownFiles()
{
    QDir dir(m_watchPath);
    if (!dir.exists()) return;

    QStringList files = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    m_knownFiles = QSet<QString>(files.begin(), files.end());
}

// void DirectoryWatcherWorker::syncFiles()
// {
//     while (true) {
//         QList<Arquivo> arquivos = m_arquivoRepository->carregarArquivosPendentes();
//         for (auto arquivo: arquivos) {
//             emit fileAdded(arquivo.caminhoArquivo());
//         }

//         QMutexLocker locker(&m_mutex);

//         if (m_stop) break;

//         bool shouldStop = m_condition.wait(&m_mutex, 60000);

//         if (shouldStop && m_stop) break;
//     }
// }

void DirectoryWatcherWorker::startService()
{
    if (m_isRunning) return;

    QDir dir(m_watchPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // TODO: Percorrer a pasta configurada e verificar se não existe algum arquivo não processado

    updateKnownFiles();
    m_watcher = new QFileSystemWatcher(this);
    m_watcher->addPath(m_watchPath);

    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, &DirectoryWatcherWorker::onDirectoryChanged);

    m_isRunning = true;
    emit serviceStarted();
    emit progressUpdate(100, QString("Monitorando o diretório: %1").arg(m_watchPath));

    qInfo() << "[DirectoryWatcher] Monitoramento iniciado na pasta" << m_watchPath << "| Thread " << QThread::currentThreadId();
}

void DirectoryWatcherWorker::stopService()
{
    if (!m_isRunning) return;
    IService::stopService();

    if (m_watcher) {
        m_watcher->removePath(m_watchPath);
        delete m_watcher;
        m_watcher = nullptr;
    }

    m_isRunning = false;
    emit serviceStopped();

    qInfo() << "[DirectoryWatcher] Serviço de monitoramento encerrado.";
}

void DirectoryWatcherWorker::onDirectoryChanged(const QString &path)
{
    QMutexLocker locker(&m_mutex);

    QDir dir(path);
    if (!dir.exists()) return;

    QStringList currentFilesList = dir.entryList(QStringList() << "*.xml" << "*.json", QDir::Files | QDir::NoDotAndDotDot);
    QSet<QString> currentFiles(currentFilesList.begin(), currentFilesList.end());

    QSet<QString> newFiles = currentFiles - m_knownFiles;
    for (const QString &fileName: newFiles) {
        QString absolutePath = dir.absoluteFilePath(fileName);

        qInfo() << "[DirectoryWatcher] Novo arquivo detectado:" << absolutePath;

        emit fileAdded(absolutePath);
        emit progressUpdate(100, QString("Novo arquivo encontrado: %1").arg(fileName));
    }

    m_knownFiles = currentFiles;
}