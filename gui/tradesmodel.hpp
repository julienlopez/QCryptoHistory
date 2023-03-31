#pragma once

#include "vectormodel.hpp"

#include "libcryptohistory/trade.hpp"

class TradesModel : public VectorModel<LibCryptoHistory::Trade>
{
public:
    explicit TradesModel(QObject* parent = nullptr);

    ~TradesModel() override = default;

    QVariant dataFromEntry(const LibCryptoHistory::Trade& trade, const int column_index, const int role) const override;

private:
    static const QColor c_green_color;
    static const QColor c_yellow_color;
    static const QColor c_red_color;
};
