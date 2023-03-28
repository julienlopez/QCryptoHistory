#pragma once

#include <vector>

namespace LibCryptoHistory
{

struct Trade;
struct Transaction;

namespace TradeComputer
{

    std::vector<Trade> analyze(std::vector<Transaction> transactions);

}

} // namespace LibCryptoHistory
