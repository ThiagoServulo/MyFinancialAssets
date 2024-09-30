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
