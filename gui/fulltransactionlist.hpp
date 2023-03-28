#pragma once

#include <QTableView>

namespace LibCryptoHistory
{
class Database;
}

class FullTransactionModel;

class FullTransactionList : public QTableView
{
    Q_OBJECT

public:
    explicit FullTransactionList(LibCryptoHistory::Database& database, QWidget* parent = nullptr);

    ~FullTransactionList() override = default;

public slots:
    void updateFromDatabase();

private:
    FullTransactionModel* m_model;
};