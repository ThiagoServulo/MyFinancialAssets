#include "asset.h"

Asset::Asset()
{

}

Asset::Asset(QString ticker, AssetType assetType, double currentPrice)
{
    this->ticker = ticker;
    this->assetType = assetType;
    this->currentPrice = currentPrice;
}

QString Asset::getTicker()
{
    return this->ticker;
}

AssetType Asset::getAssetType()
{
    return this->assetType;
}

double Asset::getCurrentPrice()
{
    return this->currentPrice;
}
