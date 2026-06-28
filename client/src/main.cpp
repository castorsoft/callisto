#include <iostream>
#include <memory>

#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QPixmap>
#include <QDebug>
#include <string>
#include <QException>

#include "forms/MainWindow.h"

int main(int argc, char *argv[]) {
    qDebug() << "Iniciando aplicação";
    QApplication app(argc, argv);

    MainWindow window;

    window.show();

    return app.exec();
}