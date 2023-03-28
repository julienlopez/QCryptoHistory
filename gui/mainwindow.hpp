#pragma once

#include <QMainWindow>

namespace LibCryptoHistory
{
    class Database;
}

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(LibCryptoHistory::Database& database, QWidget* parent = nullptr);

    ~MainWindow() override = default;

};