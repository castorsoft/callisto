#include <iostream>
#include <QCoreApplication>
#include "database/database.h"

int main(int argc, char *argv[]) {

    QCoreApplication app(argc, argv);

    std::cout << "Hello World" << std::endl;
    DatabaseManager::instance().open("QPSQL", "earth.castorsoft.com.br", "5433", "dfe", "dfe", "yametekudasai");
    auto db = DatabaseManager::instance().database();

    bool ok = db.open();

    if (ok) {
        std::cout << "ok" << std::endl;
    }

    return 0;
}