#pragma once

#include <filesystem>

#include <tl/expected.hpp>

#include <unqlitepp/unqlitepp.hpp>

namespace LibCryptoHistory
{

struct Transaction;

template<class T>
using Result = tl::expected<T, std::string>;

class Database
{
public:
    Database() = delete;

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    Database(Database&&) = default;
    Database& operator=(Database&&) = default;

    ~Database() = default;

    /**
     * @brief open a local database.
     */
    static Result<Database> open(const std::filesystem::path& file);

    /**
     * @brief open a database in memory, for test purposes.
     */
    static Result<Database> open();

    Result<std::vector<std::string>> currencies();

    Result<bool> contains(const std::string& currency);

    Result<void> addTransaction(const Transaction& transaction);

    Result<std::vector<Transaction>> transactions(const std::string& currency);

    // unqlitepp::Database& db();

private:
    unqlitepp::Database m_db;

    Database(unqlitepp::Database db);

    Result<void> addCurrency(const std::string& currency);
};

} // namespace LibCryptoHistory
