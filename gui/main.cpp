#include "mainwindow.hpp"

#include <libcryptohistory/database.hpp>

#include <QApplication>

#include <iostream>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    auto db = LibCryptoHistory::Database::open("./crypto.db");
    if (not db)
    {
        std::cout << "Unable to open database : " << db.error() << std::endl;
        return 1;
    }
 
    MainWindow w{*db};
    w.showMaximized();

    return app.exec();
}
