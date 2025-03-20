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
    // Check assets
    for(auto asset: assets)
    {
        // Check ticker
        if(asset->getTicker() == ticker)
        {
            return asset;
        }
    }

    qDebug() << "Error to get asset";
    return nullptr;
}

int InvestmentController::getTotalQuantityOfAssets(AssetType *assetType, QDate *init, QDate *end)
{
    // Init quantity
    int quantity = 0;

    // Check assets
    for(auto asset: assets)
    {
        // Get asset type
        AssetType type = asset->getAssetType();

        // Check asset type
        if(assetType == nullptr || type == *assetType)
        {
            quantity += asset->getQuantity(init, end);
        }
    }

    return quantity;
}

double InvestmentController::getTotalYieldOfAssets(AssetType *assetType, QDate *init, QDate *end)
{
    // Init total
    double total = 0;

    // Check assets
    for(auto asset: assets)
    {
        // Get asset type
        AssetType type = asset->getAssetType();

        // Check asset type
        if(assetType == nullptr || type == *assetType)
        {
            total += asset->getTotalYield(init, end);
        }
    }

    return total;
}

double InvestmentController::getTotalInvestedOfAssets(AssetType *assetType, QDate *init, QDate *end)
{
    // Init total
    double total = 0;

    // Check assets
    for(auto asset: assets)
    {
        // Get asset type
        AssetType type = asset->getAssetType();

        // Check values
        if((assetType == nullptr) || (type == *assetType && asset->getQuantity(init, end) != 0))
        {
            total += asset->getTotalInvested(init, end);
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
        // Check asset type
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
    double totalAssetInvested = (getAsset(ticker)->getTotalInvested(nullptr, nullptr));

    // Get total invested according to asset type
    AssetType assetType = getAsset(ticker)->getAssetType();
    double totalInvested = getTotalInvestedOfAssets(&assetType, nullptr, nullptr);

    // Return asset distribution in percentage
    return (getAsset(ticker)->getQuantity(nullptr, nullptr) == 0) ? 0 : (totalAssetInvested / totalInvested) * 100;
}

void InvestmentController::addFixedIncome(std::shared_ptr<FixedIncome> fixedIncome)
{
    fixedIncomes.push_back(fixedIncome);
}

std::vector<std::shared_ptr<FixedIncome>> InvestmentController::getFixedIncomes()
{
    return fixedIncomes;
}

std::vector<std::shared_ptr<FixedIncome>> InvestmentController::getSoldFixedIncomes(QDate *limitDate)
{
    std::vector<std::shared_ptr<FixedIncome>> ret;

    for(auto fixedIncome: fixedIncomes)
    {
        if(fixedIncome->getLimitDate().month() == limitDate->month() &&
                fixedIncome->getLimitDate().year() == limitDate->year() &&
                fixedIncome->getStatus() == FixedIncome::CLOSED)
        {
            ret.push_back(fixedIncome);
        }
    }

    return ret;
}

std::vector<std::shared_ptr<FixedIncome>> InvestmentController::getPurchasedFixedIncomes(QDate *initDate)
{
    std::vector<std::shared_ptr<FixedIncome>> ret;

    for(auto fixedIncome: fixedIncomes)
    {
        if(fixedIncome->getPurchaseDate().month() == initDate->month() &&
                fixedIncome->getPurchaseDate().year() == initDate->year())
        {
            ret.push_back(fixedIncome);
        }
    }

    return ret;
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

    // Check fixed incomes
    for(auto fixedIncome: fixedIncomes)
    {
        // Check fixed income status
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

    // Check fixed incomes
    for(auto fixedIncome: fixedIncomes)
    {
        // Check fixed income status
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
            getFixedIncomeTotalInvested(status)) * 100;
}

double InvestmentController::getTotalCapitalGainOfAssetsSold()
{
    // Return total cpital gain
    return (getTotalYieldOfAssetsSold() +
            getTransactionsTotalOfAssetsSold(TransactionType::VENDA) +
            getTransactionsTotalOfAssetsSold(TransactionType::RESTITUICAO) -
            getTransactionsTotalOfAssetsSold(TransactionType::COMPRA));
}

double InvestmentController::getTotalYieldOfAssetsSold()
{
    // Init total
    double total = 0;

    // Check assets
    for(auto asset: assets)
    {
        // Check asset quantity
        if(asset->getQuantity(nullptr, nullptr) == 0)
        {
            total += asset->getTotalYield(nullptr, nullptr);
        }
    }

    // Return total
    return total;
}

double InvestmentController::getTransactionsTotalOfAssetsSold(TransactionType transactionType)
{
    // Init total
    double total = 0;

    // Check assets
    for(auto asset: assets)
    {
        // Check asset quantity
        if(asset->getQuantity(nullptr, nullptr) == 0)
        {
            total += asset->getTransactionsTotal(transactionType, nullptr, nullptr);
        }
    }

    // Return total
    return total;
}

double InvestmentController::getProfitPercentageTotalOfAssetsSold()
{
    // Return profit percentage
    return (getTransactionsTotalOfAssetsSold(TransactionType::COMPRA) == 0) ? 0 :
           (getTotalCapitalGainOfAssetsSold() /
            (getTransactionsTotalOfAssetsSold(TransactionType::COMPRA) -
             getTransactionsTotalOfAssetsSold(TransactionType::RESTITUICAO))) * 100;
}

void InvestmentController::addFinancialInstitution(std::shared_ptr<FinancialInstitution> financialInstitution)
{
    financialInstitutions.push_back(financialInstitution);
}

std::vector<std::shared_ptr<FinancialInstitution>> InvestmentController::getFinancialInstitutions()
{
    return financialInstitutions;
}

std::shared_ptr<FinancialInstitution> InvestmentController::getFinancialInstitution(QString name)
{
    for(auto financialInstitution: financialInstitutions)
    {
        if(financialInstitution->getName() == name)
        {
            return financialInstitution;
        }
    }

    return nullptr;
}

void InvestmentController::removeTickerTransaction(QString ticker, Transaction transaction)
{
    for (auto& asset : assets)
    {
        if (asset->getTicker() == ticker)
        {
            asset->removeTransaction(transaction);
            return;
        }
    }
}

void InvestmentController::removeTickerYield(QString ticker, Yield yield)
{
    for (auto& asset : assets)
    {
        if (asset->getTicker() == ticker)
        {
            asset->removeYield(yield);
            return;
        }
    }
}

void InvestmentController::removeAsset(QString ticker)
{
    assets.erase(std::remove_if(assets.begin(), assets.end(),
                                [&](const std::shared_ptr<Asset>& asset)
                                {
                                    return asset->getTicker() == ticker;
                                }),
                 assets.end());
}
