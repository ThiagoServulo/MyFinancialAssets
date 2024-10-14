#include "asset.h"
#include "database.h"
#include "assetapi.h"

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

int Asset::getQuantity()
{
    Database database;
    return database.getTickerQuantity(this->ticker);
}

double Asset::getTotalYield()
{
    Database database;
    return database.getTickerTotalYield(this->ticker);
}

double Asset::getAveragePrice()
{
    Database database;
    return database.getTickerAveragePrice(this->ticker);
}
