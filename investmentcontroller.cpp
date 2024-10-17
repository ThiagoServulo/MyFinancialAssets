#include "investmentcontroller.h"
#include "database.h"
#include "assetapi.h"

InvestmentController::InvestmentController()
{

}

void InvestmentController::addAsset(std::shared_ptr<Asset> asset)
{
    assetList.push_back(asset);
}

void InvestmentController::addAssets(std::vector<Asset>& assets)
{
    for (Asset& asset : assets)
    {
        assetList.push_back(std::make_shared<Asset>(asset));
    }
}

std::vector<std::shared_ptr<Asset>> InvestmentController::getAllAssets()
{
    return assetList;
}

std::shared_ptr<Asset> InvestmentController::getAsset(QString ticker)
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

double InvestmentController::getAssetDistribution(QString ticker)
{
    Database database;
    double totalInvested = 0;

    // Get total asset invested
    double totalAssetInvested = (this->getAsset(ticker)->getQuantity() * database.getTickerAveragePrice(ticker));

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
            totalInvested += (this->getAsset(ticker)->getQuantity() * database.getTickerAveragePrice(asset->getTicker()));
        }
    }

    // Return asset distribution in percentage
    return (totalAssetInvested/totalInvested) * 100;
}

