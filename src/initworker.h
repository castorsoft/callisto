#pragma once
#include <QObject>
#include <QString>

class InitWorker : public QObject {
    Q_OBJECT
public:
    explicit InitWorker(QObject *parent = nullptr);
public slots:
    void run();
signals:
    void progress(const QString &message);
    void finished(bool ok, const QString &errorMessage);
};