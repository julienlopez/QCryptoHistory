#include <catch2/catch_test_macros.hpp>

#include <libcryptohistory/trade.hpp>
#include <libcryptohistory/tradecomputer.hpp>
#include <libcryptohistory/transaction.hpp>

using LibCryptoHistory::Trade;
using LibCryptoHistory::Transaction;
using LibCryptoHistory::Type;

using LibCryptoHistory::TradeComputer::analyze;

using namespace std::string_literals;

bool operator==(const Trade::Details& trade_details, const Transaction& transaction)
{
    return trade_details.amount == transaction.amount && trade_details.total_value == transaction.total_value
           && trade_details.price == transaction.price && trade_details.timestamp == transaction.timestamp;
}

TEST_CASE("Basic trade computer Analysis", "[trade_computer]")
{

    SECTION("an empty list produces an empty result")
    {
        CHECK(analyze({}).empty());
    }

    SECTION("a simple list with one open trade")
    {
        const std::vector transaction = {Transaction{.currency = "BTC"s,
                                                     .type = Type::Bought,
                                                     .amount = 1,
                                                     .total_value = 10,
                                                     .price = 10,
                                                     .timestamp = "timestamp1"}};
        const auto trades = analyze(transaction);
        REQUIRE(trades.size() == 1);
        CHECK(trades.front().currency == transaction.front().currency);
        CHECK(trades.front().opening_details == transaction.front());
        CHECK(trades.front().closing_details == std::nullopt);
    }

    SECTION("a simple list with one full trade")
    {
        const auto currency = "BTC"s;
        const std::vector transaction = {Transaction{.currency = currency,
                                                     .type = Type::Bought,
                                                     .amount = 1,
                                                     .total_value = 10,
                                                     .price = 10,
                                                     .timestamp = "timestamp1"},
                                         Transaction{.currency = currency,
                                                     .type = Type::Sold,
                                                     .amount = 1,
                                                     .total_value = 15,
                                                     .price = 15,
                                                     .timestamp = "timestamp2"}};
        const auto trades = analyze(transaction);
        REQUIRE(trades.size() == 1);
        CHECK(trades.front().currency == currency);
        CHECK(trades.front().opening_details == transaction.front());
        REQUIRE(trades.front().closing_details.has_value());
        CHECK(*trades.front().closing_details == transaction.back());

    }
}
