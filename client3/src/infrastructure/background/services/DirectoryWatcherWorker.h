#pragma once

#include "IService.h"
#include "../../../core/interfaces/IArquivoRepository.h"

#include <QFileSystemWatcher>
#include <QSet>
#include <QMutex>

class DirectoryWatcherWorker: public IService {
    Q_OBJECT

public:
    explicit DirectoryWatcherWorker(const QString &watchPath, QObject *parent = nullptr);
    ~DirectoryWatcherWorker() override;

    QString serviceName() const override { return "Serviço de Monitoramento de Arquivos"; }
    bool isRunning() const override;

public slots:
    void startService() override;
    void stopService() override;

signals:
    void fileAdded(const QString &filePath);

private slots:
    void onDirectoryChanged(const QString &path);

private:
    QFileSystemWatcher *m_watcher{nullptr};
    QString m_watchPath;
    QSet<QString> m_knownFiles;
    bool m_isRunning{false};
    QMutex m_mutex;

    void updateKnownFiles();
    // void syncFiles();
};