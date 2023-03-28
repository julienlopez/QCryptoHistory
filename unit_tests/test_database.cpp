#include <catch2/catch_test_macros.hpp>

#include <libcryptohistory/database.hpp>
#include <libcryptohistory/transaction.hpp>

// using namespace std::string_literals;

using LibCryptoHistory::Database;
using LibCryptoHistory::Transaction;
using LibCryptoHistory::Type;

TEST_CASE("Basic transaction workflow")
{
    auto db = Database::open();
    REQUIRE(db.has_value());
    CHECK(db->currencies().map(&std::vector<std::string>::empty) == true);

    const std::string btc = "BTC";
    const Transaction transaction1{btc, Type::Bought, 1'000., 100., 0.1, "timestamp1"};
    REQUIRE(db->addTransaction(transaction1).has_value());

    CHECK(db->currencies().map(&std::vector<std::string>::size) == 1);
    CHECK(db->contains(btc) == true);
    CHECK(db->transactions(btc) == std::vector{transaction1});

    
    const Transaction transaction2{btc, Type::Sold, 500., 100., 0.1, "timestamp2"};
    REQUIRE(db->addTransaction(transaction2).has_value());
    const Transaction transaction3{btc, Type::Bought, 2'000., 100., 0.1, "timestamp3"};
    REQUIRE(db->addTransaction(transaction3).has_value());

    CHECK(db->transactions(btc) == std::vector{transaction1, transaction2, transaction3});
}
