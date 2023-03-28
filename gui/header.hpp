#pragma once

#include <QWidget>

class QComboBox;
class QDateTimeEdit;
class QLineEdit;

namespace LibCryptoHistory
{
class Database;
}

class Header : public QWidget
{
    Q_OBJECT

public:
    explicit Header(LibCryptoHistory::Database& database, QWidget* parent = nullptr);

    ~Header() override = default;

signals:
    void transactionAdded();

private:
    LibCryptoHistory::Database& m_database;
    QLineEdit* m_currency_le;
    QComboBox* m_type_cb;
    QLineEdit* m_amount_le;
    QLineEdit* m_value_le;
    QLineEdit* m_price_le;
    QDateTimeEdit* m_timestamp_dte;

private slots: 
    void buttonClicked();
};
