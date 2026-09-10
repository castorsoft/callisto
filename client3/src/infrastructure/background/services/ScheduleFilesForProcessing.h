#pragma once

#include "IService.h"
#include "../../../core/entities/Arquivo.h"
#include "../../../core/interfaces/IArquivoRepository.h"

class ScheduleFilesForProcessing: public IService {
    Q_OBJECT

public:
    explicit ScheduleFilesForProcessing(QObject *parent = nullptr);
    ~ScheduleFilesForProcessing() override;

    QString serviceName() const override { return "Serviço de reprocessamento de arquivos"; }
    bool isRunning() const override;

public slots:
    void startService() override;
    void stopService() override;
    void onFileAdded(const QString &filePath);

signals:
    void requestFileProcess(Arquivo &arquivo);

private:
    // void execute();

    bool m_isRunning;
    IArquivoRepository *m_arquivoRepository;

};
