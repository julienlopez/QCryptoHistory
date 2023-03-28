#include "tradesmodel.hpp"

#include <QDateTime>

namespace
{

const static QStringList c_labels{TradesModel::tr("currency"),
                                  TradesModel::tr("amount"),
                                  TradesModel::tr("opening total value"),
                                  TradesModel::tr("closing total value"),
                                  TradesModel::tr("opening price"),
                                  TradesModel::tr("closing price"),
                                  TradesModel::tr("opening timestamp"),
                                  TradesModel::tr("closing timestamp")};

} // namespace

TradesModel::TradesModel(QObject* parent)
    : VectorModel<LibCryptoHistory::Trade>{c_labels, parent}
{
}

QVariant TradesModel::dataFromEntry(const LibCryptoHistory::Trade& transaction, const int column_index) const
{
    switch(column_index)
    {
    case 0:
        return QString::fromStdString(transaction.currency);
    case 1:
        return transaction.opening_details.amount;
    case 2:
        return transaction.opening_details.total_value;
    case 3:
        if(transaction.closing_details)
            return transaction.closing_details->total_value;
        else
            return {};
    case 4:
        return transaction.opening_details.price;
    case 5:
        if(transaction.closing_details)
            return transaction.closing_details->price;
        else
            return {};
    case 6:
        return QDateTime::fromString(QString::fromStdString(transaction.opening_details.timestamp), Qt::ISODate);
    case 7:
        if(transaction.closing_details)
            return QDateTime::fromString(QString::fromStdString(transaction.closing_details->timestamp), Qt::ISODate);
        else
            return {};
    }
    return {};
}