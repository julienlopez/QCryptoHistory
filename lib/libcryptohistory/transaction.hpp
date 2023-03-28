#pragma once

#include <string>

namespace LibCryptoHistory
{

enum class Type
{
    Bought, Sold
};

struct Transaction
{
    std::string currency;
    Type type;
    double amount;
    double total_value;
    double price;
    std::string timestamp;

    bool operator==(const Transaction&) const = default;
};

} // namespace LibCryptoHistory
