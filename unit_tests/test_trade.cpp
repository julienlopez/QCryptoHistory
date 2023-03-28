#include <catch2/catch_test_macros.hpp>

#include <libcryptohistory/trade.hpp>

using LibCryptoHistory::Trade;

TEST_CASE("Basic trade features", "[trade]")
{

    SECTION("open trade")
    {
        const Trade trade{.currency = "BTC",
                          .opening_details
                          = {.amount = 0.1, .total_value = 10., .price = 100., .timestamp = "timestamp1"}};
        CHECK(trade.isOpen());
        CHECK_FALSE(trade.isClosed());
    }

    SECTION("closed trade")
    {
        const Trade trade{
            .currency = "BTC",
            .opening_details = {.amount = 0.1, .total_value = 10., .price = 100., .timestamp = "timestamp1"},
            .closing_details
            = Trade::Details{.amount = 0.1, .total_value = 15., .price = 150., .timestamp = "timestamp2"}};
        CHECK_FALSE(trade.isOpen());
        CHECK(trade.isClosed());
    }
}
