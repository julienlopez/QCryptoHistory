#include "fulltransactionmodel.hpp"

#include <libcryptohistory/database.hpp>

#include <ranges>

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

auto loadTransactionsForCurrency(LibCryptoHistory::Database& database)
{
    return [&database](const std::vector<std::string>& currencies)
               -> tl::expected<std::vector<LibCryptoHistory::Transaction>, std::string>
    {
        std::vector<LibCryptoHistory::Transaction> res;
        for(const auto currency : currencies)
        {
            const auto transactions = database.transactions(currency);
            if(not transactions) return tl::make_unexpected(transactions.error());
            std::ranges::copy(*transactions, std::back_inserter(res));
        }
        return res;
    };
}

tl::expected<std::vector<LibCryptoHistory::Transaction>, std::string> loadTransactions(LibCryptoHistory::Database& database)
{
    return database.currencies().and_then(loadTransactionsForCurrency(database));
}

} // namespace

FullTransactionModel::FullTransactionModel(LibCryptoHistory::Database& database, QObject* parent)
    : QAbstractTableModel{parent}
    , m_database{database}
{
    updateFromDatabase();
}

int FullTransactionModel::rowCount(const QModelIndex& index) const
{
    return m_transactions.size();
}

int FullTransactionModel::columnCount(const QModelIndex& index) const
{
    return c_labels.size();
}

QVariant FullTransactionModel::data(const QModelIndex& index, int role) const
{
    if(index.isValid() && index.row() >= 0 && index.row() < rowCount() && index.column() >= 0
       && index.column() < columnCount() && role == Qt::DisplayRole)
    {
        const auto transaction = m_transactions[index.row()];
        switch(index.column())
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
    return QVariant();
}

QVariant FullTransactionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && section >= 0 && section < c_labels.size() && role == Qt::DisplayRole)
        return c_labels[section];
    else
        return QAbstractTableModel::headerData(section, orientation, role);
}

void FullTransactionModel::updateFromDatabase()
{
    beginResetModel();
    loadTransactions(m_database)
        .map([this](auto transactions) { m_transactions = std::move(transactions); })
        .or_else([this](const std::string& error)
                 { qDebug() << tr("Error loading transactions : ") + QString::fromStdString(error); });
    endResetModel();
}
