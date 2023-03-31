#pragma once

#include <QMainWindow>

class QAbstractItemModel;

namespace LibCryptoHistory
{
class Database;
}

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(LibCryptoHistory::Database& database, QWidget* parent = nullptr);

    ~MainWindow() override = default;

private:
    static QWidget* createTradesView(QAbstractItemModel* model);
};