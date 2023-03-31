#include "tradesmodel.hpp"

#include <QBrush>
#include <QColor>
#include <QDateTime>
#include <QLocale>

namespace
{

const static QStringList c_labels{TradesModel::tr("currency"),
                                  TradesModel::tr("amount"),
                                  TradesModel::tr("opening total value"),
                                  TradesModel::tr("closing total value"),
                                  TradesModel::tr("opening price"),
                                  TradesModel::tr("closing price"),
                                  TradesModel::tr("opening timestamp"),
                                  TradesModel::tr("closing timestamp"),
                                  TradesModel::tr("result")};

} // namespace

const QColor TradesModel::c_green_color{130, 179, 102};
const QColor TradesModel::c_yellow_color{255, 242, 204};
const QColor TradesModel::c_red_color{184, 84, 80};

TradesModel::TradesModel(QObject* parent)
    : VectorModel<LibCryptoHistory::Trade>{c_labels, parent}
{
}

QVariant TradesModel::dataFromEntry(const LibCryptoHistory::Trade& trade, const int column_index, const int role) const
{
    if(role == Qt::BackgroundRole)
    {
        const auto res = trade.result();
        if(res.has_value())
            return QBrush{res > 0 ? c_green_color : c_red_color};
        else
            return QBrush{c_yellow_color};
    }
    if(role != Qt::DisplayRole) return {};
    switch(column_index)
    {
    case 0:
        return QString::fromStdString(trade.currency);
    case 1:
        return trade.opening_details.amount;
    case 2:
        return trade.opening_details.total_value;
    case 3:
        if(trade.closing_details)
            return trade.closing_details->total_value;
        else
            return {};
    case 4:
        return trade.opening_details.price;
    case 5:
        if(trade.closing_details)
            return trade.closing_details->price;
        else
            return {};
    case 6:
        return QDateTime::fromString(QString::fromStdString(trade.opening_details.timestamp), Qt::ISODate);
    case 7:
        if(trade.closing_details)
            return QDateTime::fromString(QString::fromStdString(trade.closing_details->timestamp), Qt::ISODate);
        else
            return {};
    case 8:
        if(const auto res = trade.result(); res.has_value())
            return (*res > 0 ? "+" : "") + QLocale::system().toString(100 * *res) + "%";
        else
            return {};
    }
    return {};
}