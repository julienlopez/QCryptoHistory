#pragma once

#include "libcryptohistory/transaction.hpp"

#include <vector>

#include <QAbstractTableModel>

namespace LibCryptoHistory
{
class Database;
}

class FullTransactionModel : public QAbstractTableModel
{
public:
    explicit FullTransactionModel(LibCryptoHistory::Database& database, QObject* parent = nullptr);

    ~FullTransactionModel() override = default;

    int rowCount(const QModelIndex& index = {}) const override;
    
    int columnCount(const QModelIndex& index = {}) const override;
    
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void updateFromDatabase();

private:
    LibCryptoHistory::Database& m_database;
    std::vector<LibCryptoHistory::Transaction> m_transactions;
};