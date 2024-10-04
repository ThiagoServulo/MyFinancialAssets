#include "assetcontroller.h"
#include "database.h"
#include "assetapi.h"

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

std::shared_ptr<Asset> AssetController::getAsset(QString ticker)
{
    for(auto asset: assetList)
    {
        if(asset->getTicker() == ticker)
        {
            return asset;
        }
    }

    qDebug() << "Error to get asset";
    return nullptr;
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
    double totalInvested = 0;

    // Get total asset invested
    double totalAssetInvested = (database.getTickerQuantity(ticker) * database.getTickerAveragePrice(ticker));

    // Get asset required
    auto assetRequired = getAsset(ticker);

    // Check asset required
    if(assetRequired == nullptr)
    {
        return 0;
    }

    for(auto asset: assetList)
    {
        // Check asset type
        if(assetRequired->getAssetType() == asset->getAssetType())
        {
            totalInvested += (database.getTickerQuantity(asset->getTicker()) * database.getTickerAveragePrice(asset->getTicker()));
        }
    }

    // Return asset distribution in percentage
    return (totalAssetInvested/totalInvested) * 100;
}

double AssetController::getAssetTotalYield(QString ticker)
{
    Database database;
    return database.getTickerTotalYield(ticker);
}

double AssetController::getAssetCurrentPrice(QString ticker)
{
    AssetApi assetApi;
    return assetApi.getAssetCurrentPrice(ticker);
}
