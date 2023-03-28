#pragma once

#include "vectormodel.hpp"

#include "libcryptohistory/trade.hpp"

class TradesModel : public VectorModel<LibCryptoHistory::Trade>
{
public:
    explicit TradesModel(QObject* parent = nullptr);

    ~TradesModel() override = default;

    QVariant dataFromEntry(const LibCryptoHistory::Trade& trade, const int column_index) const override;
};