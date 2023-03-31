#include "fulltransactionmodel.hpp"

#include <libcryptohistory/database.hpp>

#include <QDateTime>

namespace
{

const static QStringList c_labels{FullTransactionModel::tr("currency"), FullTransactionModel::tr("type"),
                                  FullTransactionModel::tr("amount"),   FullTransactionModel::tr("total_value"),
                                  FullTransactionModel::tr("price"),    FullTransactionModel::tr("timestamp")};

QString toString(const LibCryptoHistory::Type type)
{
    switch(type)
    {
    case LibCryptoHistory::Type::Bought:
        return FullTransactionModel::tr("Bought");
    case LibCryptoHistory::Type::Sold:
        return FullTransactionModel::tr("Sold");
    }
    return "";
}

} // namespace

FullTransactionModel::FullTransactionModel(LibCryptoHistory::Database& database, QObject* parent)
    : VectorModel<LibCryptoHistory::Transaction>{c_labels, parent}
    , m_database{database}
{
}

const std::vector<LibCryptoHistory::Transaction>& FullTransactionModel::updateFromDatabase()
{
    m_database.allTransactions()
        .map(std::bind_front(&FullTransactionModel::updateData, this))
        .or_else([this](const std::string& error)
                 { qDebug() << tr("Error loading transactions : ") + QString::fromStdString(error); });
    return data();
}

QVariant FullTransactionModel::dataFromEntry(const LibCryptoHistory::Transaction& transaction, const int column_index,
                                             const int role) const
{
    if(role != Qt::DisplayRole) return {};
    switch(column_index)
    {
    case 0:
        return QString::fromStdString(transaction.currency);
    case 1:
        return toString(transaction.type);
    case 2:
        return transaction.amount;
    case 3:
        return transaction.total_value;
    case 4:
        return transaction.price;
    case 5:
        return QDateTime::fromString(QString::fromStdString(transaction.timestamp), Qt::ISODate);
    }
}