#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QThread>
#include <QMainWindow>
#include <QTimer>
#include "initworker.h"
#include <iostream>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    // Splash (pode trocar pixmap por recurso próprio)
    QPixmap pixmap(400, 200);
    pixmap.fill(Qt::white);
    QSplashScreen splash(pixmap);
    splash.showMessage("Iniciando...", Qt::AlignBottom | Qt::AlignHCenter);
    splash.show();
    app.processEvents();

    // Thread e worker
    // QThread *workerThread = new QThread;
    InitWorker *worker = new InitWorker;
    worker->moveToThread(workerThread);

    // QObject::connect(workerThread, &QThread::started, worker, &InitWorker::run);
    QObject::connect(worker, &InitWorker::progress, [&splash](const QString &msg){
        splash.showMessage(msg, Qt::AlignBottom | Qt::AlignHCenter);
    });
    QObject::connect(worker, &InitWorker::finished, [&](bool ok, const QString &error){
        if (!ok) {
            splash.showMessage(QString("Erro: %1").arg(error), Qt::AlignBottom | Qt::AlignHCenter);
            QTimer::singleShot(3000, &app, &QCoreApplication::quit);
        } else {
            // Criar janela principal e mostrar
            QMainWindow *w = new QMainWindow;
            w->setWindowTitle("DFE - Principal");
            w->resize(800, 600);
            w->show();

            splash.finish(w);
        }
        workerThread->quit();
    });
    QObject::connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    QObject::connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);

    // workerThread->start();

    return app.exec();
}