#ifndef INVESTMENTCONTROLLER_H
#define INVESTMENTCONTROLLER_H

#include "asset.h"
#include "fixedincome.h"

class InvestmentController
{
private:
    std::vector<std::shared_ptr<Asset>> assets;
    std::vector<std::shared_ptr<FixedIncome>> fixedIncomes;

public:
    InvestmentController();
    std::shared_ptr<Asset> getAsset(QString ticker);
    void addAsset(std::shared_ptr<Asset> asset);
    int getTotalQuantityOfAssets(AssetType assetType);
    double getTotalYieldOfAssets(AssetType assetType);
    double getTotalInvestedOfAssets(AssetType assetType);
    std::vector<std::shared_ptr<Asset>> getAllAssets();
    double getTotalCapitalGainOfAssets(AssetType assetType);
    double getAssetDistribution(QString ticker);
    void addFixedIncome(std::shared_ptr<FixedIncome> fixedIncome);
    std::vector<std::shared_ptr<FixedIncome>> getFixedIncomes();
    std::shared_ptr<FixedIncome> getFixedIncome(QDate purchaseDate, QString description);
    double getFixedIncomeTotalInvested(bool status);
    double getFixedIncomeCurrentTotal(bool status);
    double getFixedIncomeTotalYield(bool status);
    double getFixedIncomeTotalYieldPercentage(bool status);
    double getTotalCapitalGainOfAssetsSold();
    double getTotalYieldOfAssetsSold();
    double getPurchaseTotalOfAssetsSold();
    double getSaleTotalOfAssetsSold();
    double getProfitPercentageTotalOfAssetsSold();
};

#endif // INVESTMENTCONTROLLER_H
