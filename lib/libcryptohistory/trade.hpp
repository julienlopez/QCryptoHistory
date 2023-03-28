#pragma once

#include <string>

#include <optional>

namespace LibCryptoHistory
{

struct Trade
{
    struct Details
    {
        double amount;
        double total_value;
        double price;
        std::string timestamp;
    };

    std::string currency;
    Details opening_details;
    std::optional<Details> closing_details = std::nullopt;

    bool operator==(const Trade&) const = default;

    bool isOpen() const;

    bool isClosed() const;
};

} // namespace LibCryptoHistory
