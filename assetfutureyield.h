#ifndef ASSETFUTUREYIELD_H
#define ASSETFUTUREYIELD_H

#include <QDate>
#include "asset.h"
#include "constants.h"

class AssetFutureYield
{
public:
    AssetFutureYield(QDate exYieldDate, QDate paymentDate, YieldType type, double value, double ratio);
    QDate getExYieldDate() const;
    QDate getPaymentDate() const;
    YieldType getYieldType() const;
    double getValue() const;
    double getRatio() const;

private:
    QDate exYieldDate;
    QDate paymentDate;
    YieldType type;
    double value;
    double ratio;
};

QList<AssetFutureYield> getFutureYieldsForTicker(Asset asset);

#endif // ASSETFUTUREYIELD_H
