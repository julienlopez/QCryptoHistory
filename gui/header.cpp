#include "header.hpp"

#include <libcryptohistory/database.hpp>
#include <libcryptohistory/transaction.hpp>

#include <QComboBox>
#include <QDateTimeEdit>
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>

namespace
{

LibCryptoHistory::Type typeFromString(const QString str)
{
    if(str == Header::tr("Bought"))
        return LibCryptoHistory::Type::Bought;
    else
        return LibCryptoHistory::Type::Sold;
}

LibCryptoHistory::Transaction buildTransaction(const QLineEdit* currency_le, const QComboBox* type_cb,
                                               const QLineEdit* amount_le, const QLineEdit* value_le,
                                               const QLineEdit* price_le, const QDateTimeEdit* timestamp_dte)
{
    const auto locale = QLocale::system();
    return {.currency = currency_le->text().toStdString(),
            .type = typeFromString(type_cb->currentText()),
            .amount = locale.toDouble(amount_le->text()),
            .total_value = locale.toDouble(value_le->text()),
            .price = locale.toDouble(price_le->text()),
            .timestamp = timestamp_dte->dateTime().toString(Qt::ISODate).toStdString()};
}

} // namespace

Header::Header(LibCryptoHistory::Database& database, QWidget* parent)
    : QWidget{parent}
    , m_database{database}
{
    setFixedHeight(100);
    auto* hl = new QHBoxLayout;

    auto* double_validator = new QDoubleValidator;
    double_validator->setBottom(0.);

    hl->addWidget(new QLabel{tr("Currency : ")});
    m_currency_le = new QLineEdit;
    hl->addWidget(m_currency_le);

    hl->addWidget(new QLabel{tr("Type : ")});
    m_type_cb = new QComboBox;
    m_type_cb->addItems({tr("Bought"), tr("Sold")});
    hl->addWidget(m_type_cb);

    hl->addWidget(new QLabel{tr("Amount : ")});
    m_amount_le = new QLineEdit;
    m_amount_le->setValidator(double_validator);
    hl->addWidget(m_amount_le);

    hl->addWidget(new QLabel{tr("Total Value : ")});
    m_value_le = new QLineEdit;
    m_value_le->setValidator(double_validator);
    hl->addWidget(m_value_le);

    hl->addWidget(new QLabel{tr("Price : ")});
    m_price_le = new QLineEdit;
    m_price_le->setValidator(double_validator);
    hl->addWidget(m_price_le);

    hl->addWidget(new QLabel{tr("Timestamp : ")});
    m_timestamp_dte = new QDateTimeEdit;
    m_timestamp_dte->setCalendarPopup(true);
    hl->addWidget(m_timestamp_dte);

    auto* b = new QPushButton{tr("Add Transaction")};
    connect(b, &QPushButton::clicked, this, &Header::buttonClicked);
    hl->addWidget(b);

    setLayout(hl);
}

void Header::buttonClicked()
{
    const auto res = m_database.addTransaction(
        buildTransaction(m_currency_le, m_type_cb, m_amount_le, m_value_le, m_price_le, m_timestamp_dte));
    if(res)
    {
        emit transactionAdded();
    }
    else
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("Unable to add transaction : ") + QString::fromStdString(res.error()));
    }
}
