#include "assetcontroller.h"
#include "database.h"

AssetController::AssetController()
{

}

void AssetController::addAsset(std::shared_ptr<Asset> asset)
{
    assetList.push_back(asset);
}

void AssetController::addAssets(std::vector<Asset>& assets)
{
    for (Asset& asset : assets)
    {
        assetList.push_back(std::make_shared<Asset>(asset));
    }
}

std::vector<std::shared_ptr<Asset>> AssetController::getAllAssets()
{
    return assetList;
}

int AssetController::getAssetQuantity(QString ticker)
{
    Database database;
    return database.getTickerQuantity(ticker);
}

double AssetController::getAveragePrice(QString ticker)
{
    Database database;
    return database.getTickerAveragePrice(ticker);
}

double AssetController::getAssetDistribution(QString ticker)
{
    Database database;

    double totalAssetInvested = (database.getTickerQuantity(ticker) * database.getTickerAveragePrice(ticker));
    double totalInvested = 0;

    for(auto asset: assetList)
    {
        totalInvested += (database.getTickerQuantity(asset->getTicker()) * database.getTickerAveragePrice(asset->getTicker()));
    }

    return (totalAssetInvested/totalInvested) * 100;
}

double AssetController::getAssetTotalYield(QString ticker)
{
    Database database;
    return database.getTickerTotalYield(ticker);
}
