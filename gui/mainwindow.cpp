#include "mainwindow.hpp"

#include "header.hpp"
#include "fulltransactionlist.hpp"

#include <QVBoxLayout>

MainWindow::MainWindow(LibCryptoHistory::Database& database, QWidget* parent)
    : QMainWindow(parent)
{
    auto* w = new QWidget;
    auto* vl = new QVBoxLayout;
    auto* header = new Header{database};
    vl->addWidget(header);
    auto* full_tr_list = new FullTransactionList{database};
    vl->addWidget(full_tr_list);
    w->setLayout(vl);
    setCentralWidget(w);
    connect(header, &Header::transactionAdded, full_tr_list, &FullTransactionList::updateFromDatabase);
}
