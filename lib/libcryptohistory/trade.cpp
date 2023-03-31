#include "trade.hpp"

namespace LibCryptoHistory
{

bool Trade::isOpen() const
{
    return closing_details == std::nullopt;
}

bool Trade::isClosed() const
{
    return closing_details.has_value();
}

std::optional<double> Trade::result() const
{
    if(not closing_details.has_value()) return std::nullopt;
    const auto diff = closing_details->total_value - opening_details.total_value;
    return diff / opening_details.total_value;
}

} // namespace LibCryptoHistory
