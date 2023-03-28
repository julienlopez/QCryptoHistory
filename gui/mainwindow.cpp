#include "mainwindow.hpp"

#include "header.hpp"
#include "fulltransactionlist.hpp"

#include <QTabWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(LibCryptoHistory::Database& database, QWidget* parent)
    : QMainWindow(parent)
{
    auto* w = new QWidget;
    auto* vl = new QVBoxLayout;
    auto* header = new Header{database};
    vl->addWidget(header);
    auto* tab_widget = new QTabWidget;
    vl->addWidget(tab_widget);
    w->setLayout(vl);
    setCentralWidget(w);

    auto* full_tr_list = new FullTransactionList{database};
    tab_widget->addTab(full_tr_list, tr("Full listing"));
    connect(header, &Header::transactionAdded, full_tr_list, &FullTransactionList::updateFromDatabase);
}
