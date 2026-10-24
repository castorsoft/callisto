#pragma once

#include <QObject>
#include <QString>
#include <QMutex>
#include <QWaitCondition>

class IService: public QObject {
    Q_OBJECT

public:
    explicit IService(QObject *parent = nullptr): QObject(parent) {};
    virtual ~IService() = default;

    virtual QString serviceName() const = 0;
    virtual bool isRunning() const = 0;
public slots:
    virtual void startService() = 0;
    virtual void stopService()
    {
        QMutexLocker locker(&m_mutex);
        m_stop = true;
        m_condition.wakeAll();
    };

signals:
    void serviceStarted();
    void serviceStopped();
    void progressUpdate(int percentage, const QString &statusMessage);
    void errorOcurred(const QString &errorMessage);

protected:
    QMutex m_mutex;
    QWaitCondition m_condition;
    bool m_stop = false;
};