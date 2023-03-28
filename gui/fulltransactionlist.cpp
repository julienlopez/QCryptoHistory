#include "fulltransactionlist.hpp"

#include "fulltransactionmodel.hpp"

FullTransactionList::FullTransactionList(LibCryptoHistory::Database& database, QWidget* parent)
    : QTableView{parent}
{
    m_model = new FullTransactionModel{database, this};
    setModel(m_model);
}

void FullTransactionList::updateFromDatabase()
{
    m_model->updateFromDatabase();
}
