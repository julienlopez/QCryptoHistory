#pragma once

#include "vectormodel.hpp"

#include "libcryptohistory/transaction.hpp"

namespace LibCryptoHistory
{
class Database;
}

class FullTransactionModel : public VectorModel<LibCryptoHistory::Transaction>
{
public:
    explicit FullTransactionModel(LibCryptoHistory::Database& database, QObject* parent = nullptr);

    ~FullTransactionModel() override = default;

    const std::vector<LibCryptoHistory::Transaction>& updateFromDatabase();

    QVariant dataFromEntry(const LibCryptoHistory::Transaction& transaction, const int column_index,
                           const int role) const override;

private:
    LibCryptoHistory::Database& m_database;
};