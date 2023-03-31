#include "mainwindow.hpp"

#include "fulltransactionmodel.hpp"
#include "header.hpp"
#include "tradesmodel.hpp"

#include "libcryptohistory/tradecomputer.hpp"

#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QTabWidget>
#include <QTableView>
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

    auto* trades_model = new TradesModel{this};
    auto* trades_list = new QTableView;
    auto* trades_proxy_model = new QSortFilterProxyModel;
    trades_proxy_model->setSourceModel(trades_model);
    trades_list->setModel(trades_proxy_model);
    trades_list->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    trades_list->setSortingEnabled(true);
    trades_list->sortByColumn(6, Qt::DescendingOrder);
    tab_widget->addTab(trades_list, tr("Trades"));

    const auto update_models = [full_tr_model, trades_model]()
    {
        const auto& transactions = full_tr_model->updateFromDatabase();
        trades_model->updateData(LibCryptoHistory::TradeComputer::analyze(transactions));
    };
    connect(header, &Header::transactionAdded, update_models);
    update_models();
}
