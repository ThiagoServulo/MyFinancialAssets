#include "investmentcontroller.h"
#include "database.h"
#include "assetapi.h"

InvestmentController::InvestmentController()
{

}

void InvestmentController::addAsset(std::shared_ptr<Asset> asset)
{
    assets.push_back(asset);
}

std::vector<std::shared_ptr<Asset>> InvestmentController::getAllAssets()
{
    return assets;
}

std::shared_ptr<Asset> InvestmentController::getAsset(QString ticker)
{
    for(auto asset: assets)
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
    double totalInvested = 0;

    // Get total asset invested
    double totalAssetInvested = (this->getAsset(ticker)->getQuantity() * this->getAsset(ticker)->getAveragePrice());

    // Get asset required
    auto assetRequired = getAsset(ticker);

    // Check asset required
    if(assetRequired == nullptr)
    {
        return 0;
    }

    for(auto asset: assets)
    {
        // Check asset type
        if(assetRequired->getAssetType() == asset->getAssetType())
        {
            totalInvested += (this->getAsset(ticker)->getQuantity() * this->getAsset(ticker)->getAveragePrice());
        }
    }

    // Return asset distribution in percentage
    return (totalAssetInvested/totalInvested) * 100;
}

