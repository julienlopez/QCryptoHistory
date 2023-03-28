#include "tradecomputer.hpp"

#include "trade.hpp"
#include "transaction.hpp"

#include <algorithm>

namespace LibCryptoHistory
{

using TransactionContainer = std::vector<Transaction>;

TransactionContainer::const_iterator findOpeningOfATrade(const TransactionContainer& transactions)
{
    return std::ranges::find_if(transactions,
                                [](const Transaction& transaction) { return transaction.type == Type::Bought; });
}

bool approx(const double v1, const double v2)
{
    return std::abs(v2 - v1) < v2 * 0.0001;
}

auto isMatchingClose(const Transaction& opening)
{
    return [&opening](const Transaction& transaction) 
    {
        return opening.currency == transaction.currency && transaction.type == Type::Sold
               && approx(transaction.amount, opening.amount);
    };
}

TransactionContainer::const_iterator findCorrespondingClosingTransaction(const TransactionContainer& transactions,
                                                                         const Transaction& opening)
{
    return std::ranges::find_if(transactions, isMatchingClose(opening));
}

Trade::Details detailsFromTransaction(const Transaction& transaction)
{
    return {.amount = transaction.amount,
            .total_value = transaction.total_value,
            .price = transaction.price,
            .timestamp = transaction.timestamp};
}

std::vector<Trade> TradeComputer::analyze(std::vector<Transaction> transactions)
{
    if(transactions.empty()) return {};
    std::ranges::sort(transactions, {}, &Transaction::timestamp);
    std::ranges::stable_sort(transactions, {}, &Transaction::currency);

    std::vector<Trade> trades;
    auto it_opening = findOpeningOfATrade(transactions);
    while(it_opening != end(transactions))
    {
        const auto close_it = findCorrespondingClosingTransaction(transactions, *it_opening);
        if(close_it == end(transactions))
        {
            trades.emplace_back(it_opening->currency, detailsFromTransaction(*it_opening));
            transactions.erase(it_opening);
        }
        else
        {
            trades.emplace_back(it_opening->currency, detailsFromTransaction(*it_opening),
                                detailsFromTransaction(*close_it));
            transactions.erase(close_it);
            transactions.erase(it_opening);
        }
        it_opening = findOpeningOfATrade(transactions);
    }
    return trades;
}

} // namespace LibCryptoHistory
