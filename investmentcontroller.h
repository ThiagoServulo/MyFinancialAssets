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
    double getAssetDistribution(QString ticker);
    void addFixedIncome(std::shared_ptr<FixedIncome> fixedIncome);
    std::vector<std::shared_ptr<FixedIncome>> getFixedIncomes(bool status);
    std::shared_ptr<FixedIncome> getFixedIncome(QDate purchaseDate, QString description);
};

#endif // INVESTMENTCONTROLLER_H
