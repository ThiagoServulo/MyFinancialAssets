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

int InvestmentController::getTotalQuantityOfAssets(AssetType assetType)
{
    // Init quantity
    int quantity = 0;

    // Check assets
    for(auto asset: assets)
    {
        if(asset->getAssetType() == assetType)
        {
            quantity += asset->getQuantity();
        }
    }

    return quantity;
}

double InvestmentController::getTotalYieldOfAssets(AssetType assetType)
{
    // Init total
    double total = 0;

    // Check assets
    for(auto asset: assets)
    {
        if(asset->getAssetType() == assetType)
        {
            total += asset->getTotalYield();
        }
    }

    return total;
}

double InvestmentController::getTotalInvestedOfAssets(AssetType assetType)
{
    // Init total
    double total = 0;

    // Check assets
    for(auto asset: assets)
    {
        if(asset->getAssetType() == assetType && asset->getQuantity() != 0)
        {
            total += asset->getTotalInvested();
        }
    }

    // Return total
    return total;
}

double InvestmentController::getTotalCapitalGainOfAssets(AssetType assetType)
{
    // Init total
    double total = 0;

    // Check assets
    for(auto asset: assets)
    {
        if(asset->getAssetType() == assetType)
        {
            total += asset->getCapitalGain();
        }
    }

    // Return total
    return total;
}

double InvestmentController::getAssetDistribution(QString ticker)
{
    // Get total asset invested
    double totalAssetInvested = (getAsset(ticker)->getTotalInvested());

    // Get total invested according to asset type
    double totalInvested = getTotalInvestedOfAssets(getAsset(ticker)->getAssetType());

    // Return asset distribution in percentage
    return (getAsset(ticker)->getQuantity() == 0) ? 0 : (totalAssetInvested / totalInvested) * 100;
}

void InvestmentController::addFixedIncome(std::shared_ptr<FixedIncome> fixedIncome)
{
    fixedIncomes.push_back(fixedIncome);
}

std::vector<std::shared_ptr<FixedIncome>> InvestmentController::getFixedIncomes()
{
    return fixedIncomes;
}

std::shared_ptr<FixedIncome> InvestmentController::getFixedIncome(QDate purchaseDate, QString description)
{
    for(auto fixedIncome: fixedIncomes)
    {
        if(fixedIncome->getPurchaseDate() == purchaseDate && fixedIncome->getDescription() == description)
        {
            return fixedIncome;
        }
    }

    qDebug() << "Error to get fixed income";
    return nullptr;
}

double InvestmentController::getFixedIncomeTotalInvested(bool status)
{
    // Init total
    double total = 0;

    for(auto fixedIncome: fixedIncomes)
    {
        if(fixedIncome->getStatus() == status)
        {
            total += fixedIncome->getInvestedValue();
        }
    }

    // Return total
    return total;
}

double InvestmentController::getFixedIncomeCurrentTotal(bool status)
{
    // Init total
    double total = 0;

    for(auto fixedIncome: fixedIncomes)
    {
        if(fixedIncome->getStatus() == status)
        {
            total += fixedIncome->getCurrentValue();
        }
    }

    // Return total
    return total;
}

double InvestmentController::getFixedIncomeTotalYield(bool status)
{
    // Return total yield
    return getFixedIncomeCurrentTotal(status) - getFixedIncomeTotalInvested(status);
}

double InvestmentController::getFixedIncomeTotalYieldPercentage(bool status)
{
    // Return total yield percentage
    return ((getFixedIncomeCurrentTotal(status) - getFixedIncomeTotalInvested(status)) /
            getFixedIncomeCurrentTotal(status)) * 100;
}
