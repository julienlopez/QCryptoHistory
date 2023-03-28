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

} // namespace LibCryptoHistory
