#include "mainwindow.hpp"

#include "header.hpp"
#include "fulltransactionmodel.hpp"

#include <QTableView>
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

    auto* full_tr_model = new FullTransactionModel{database, this};
    auto* full_tr_list = new QTableView;
    full_tr_list->setModel(full_tr_model);
    tab_widget->addTab(full_tr_list, tr("Full listing"));
    connect(header, &Header::transactionAdded, full_tr_model, &FullTransactionModel::updateFromDatabase);
}
