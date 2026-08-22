#include "DirectoryWatcherWorker.h"

#include <QDir>

DirectoryWatcherWorker::DirectoryWatcherWorker(const QString &watchPath, QObject *parent): IService(parent), m_watchPath(watchPath)
{
    this->moveToThread(&m_thread);
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

void DirectoryWatcherWorker::startService()
{
    if (m_isRunning) return;

    connect(&m_thread, &QThread::started, this, [this]() {
        QDir dir(m_watchPath);
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        updateKnownFiles();
        m_watcher = new QFileSystemWatcher(this);
        m_watcher->addPath(m_watchPath);

        connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, &DirectoryWatcherWorker::onDirectoryChanged);

        m_isRunning = true;
        emit serviceStarted();
        emit progressUpdate(100, QString("Monitorando o diretório: %1").arg(m_watchPath));

        qInfo() << "[DirectoryWatcher] Monitoramento iniciado na pasta" << m_watchPath << "| Thread " << QThread::currentThreadId();
    });

    m_thread.start();
}

void DirectoryWatcherWorker::stopService()
{
    if (!m_isRunning) return;

    if (m_watcher) {
        m_watcher->removePath(m_watchPath);
        delete m_watcher;
        m_watcher = nullptr;
    }

    m_thread.quit();
    m_thread.wait();

    m_isRunning = false;
    emit serviceStopped();

    qInfo() << "[DirectoryWatcher] Serviço de monitoramento encerrado.";
}

void DirectoryWatcherWorker::onDirectoryChanged(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) return;

    QStringList current
}