#ifndef ISERVICE_H
#define ISERVICE_H

#include <QObject>
#include <QString>

class IService: public QObject {
    Q_OBJECT

public:
    explicit IService(QObject *parent = nullptr): QObject(parent) {};
    virtual ~IService() = default;

    virtual QString serviceName() const = 0;
    virtual bool isRunning() const = 0;

public slots:
    virtual void startService() = 0;
    virtual void stopService() = 0;

signals:
    void serviceStarted();
    void serviceStopped();
    void progressUpdate(int percentage, const QString &statusMessage);
    void errorOcurred(const QString &errorMessage);
};

#endif