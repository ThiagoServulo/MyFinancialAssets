#ifndef ASSET_H
#define ASSET_H

#include "constants.h"
#include "eventcontroller.h"
#include <QString>

class Asset: public EventController
{
private:
    QString ticker;
    AssetType assetType;
    double currentPrice;

public:
    Asset();
    Asset(QString ticker, AssetType assetType, double currentPrice);
    QString getTicker();
    AssetType getAssetType();
    double getCurrentPrice();
};

#endif // ASSET_H
