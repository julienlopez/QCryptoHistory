#include "database.hpp"

#include "transaction.hpp"

using namespace std::string_literals;

namespace LibCryptoHistory
{

namespace
{

    constexpr const auto index_entry = "currencies.index";

    std::string toString(unqlitepp::Error error)
    {
        return std::to_string(error.code) + " : " + error.log;
    }

    Result<unqlitepp::Database> setupIndexIfNeeded(unqlitepp::Database db)
    {
        const auto content = unqlitepp::fetch(db, index_entry);
        if(not content)
        {
            const auto res = unqlitepp::store(db, index_entry, "");
            if(not res) return tl::make_unexpected(toString(res.error()));
        }
        return db;
    }

    std::string serialize(const Transaction& transaction)
    {
        return (transaction.type == Type::Bought ? "B" : "S") + ","s + std::to_string(transaction.amount) + ","
               + std::to_string(transaction.total_value) + "," + std::to_string(transaction.price) + ","
               + transaction.timestamp + "\n";
    }

    std::vector<std::string> split(const std::string& str, const char delimiter)
    {
        std::vector<std::string> res;
        std::istringstream iss{str};
        std::string s;
        while(std::getline(iss, s, delimiter))
            res.push_back(std::move(s));
        return res;
    }

    std::vector<std::string> splitByLine(const std::string& str)
    {
        return split(str, '\n');
    }

    Result<Type> parseType(const std::string& str)
    {
        if(str == "B") return Type::Bought;
        if(str == "S") return Type::Sold;
        return tl::make_unexpected("Invalid type : " + str);
    }

    Result<Transaction> parseTransaction(const std::string& line, const std::string& currency)
    {
        const auto parts = split(line, ',');
        if(parts.size() != 5) return tl::make_unexpected("invalid transaction found : " + line);
        return parseType(parts.front())
            .map(
                [&currency, &parts](const Type& type)
                {
                    return Transaction{.currency = currency,
                                       .type = type,
                                       .amount = std::stod(parts[1]),
                                       .total_value = std::stod(parts[2]),
                                       .price = std::stod(parts[3]),
                                       .timestamp = parts[4]};
                });
    }

    auto parseTransactions(const std::string& currency)
    {
        return [&currency](const std::vector<std::string>& lines) -> Result<std::vector<Transaction>>
        {
            std::vector<Transaction> res;
            res.reserve(lines.size());
            for(const auto& line : lines)
            {
                const auto tr = parseTransaction(line, currency);
                if(tr)
                    res.push_back(*tr);
                else
                    return tl::make_unexpected(tr.error());
            }
            return res;
        };
    }

} // namespace

Result<Database> Database::open(const std::filesystem::path& file)
{
    return unqlitepp::openLocalFile(file, unqlitepp::OpenMode::Create)
        .map_error(toString)
        .and_then(setupIndexIfNeeded)
        .map([](unqlitepp::Database db) { return Database{std::move(db)}; });
}

Result<Database> Database::open()
{
    return unqlitepp::openInMemory(unqlitepp::OpenMode::ReadWrite)
        .map_error(toString)
        .and_then(setupIndexIfNeeded)
        .map([](unqlitepp::Database db) { return Database{std::move(db)}; });
}

Result<std::vector<std::string>> Database::currencies()
{
    return unqlitepp::fetch(m_db, index_entry).map(splitByLine).map_error(toString);
}

Result<bool> Database::contains(const std::string& currency)
{
    return currencies().map([&currency](const auto& currencies)
                            { return std::ranges::find(currencies, currency) != end(currencies); });
}

Result<void> Database::addTransaction(const Transaction& transaction)
{
    return addCurrency(transaction.currency)
        .and_then(
            [this, &transaction]()
            { return unqlitepp::append(m_db, transaction.currency, serialize(transaction)).map_error(toString); });
}

Result<std::vector<Transaction>> Database::transactions(const std::string& currency)
{
    return contains(currency).and_then(
        [this, &currency](const bool already_present) -> Result<std::vector<Transaction>>
        {
            if(not already_present) return {};
            return unqlitepp::fetch(m_db, currency)
                .map_error(toString)
                .map(splitByLine)
                .and_then(parseTransactions(currency));
        });
}

// unqlitepp::Database& Database::db()
// {
//     return m_db;
// }

Database::Database(unqlitepp::Database db)
    : m_db{std::move(db)}
{
}

Result<void> Database::addCurrency(const std::string& currency)
{
    return contains(currency).and_then(
        [this, &currency](const bool already_present) -> Result<void>
        {
            if(already_present) return {};
            return unqlitepp::append(m_db, index_entry, currency + "\n").map_error(toString);
        });
}

} // namespace LibCryptoHistory
