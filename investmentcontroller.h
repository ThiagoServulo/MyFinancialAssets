#ifndef INVESTMENTCONTROLLER_H
#define INVESTMENTCONTROLLER_H

#include "asset.h"

class InvestmentController
{
private:
    std::vector<std::shared_ptr<Asset>> assetList;

public:
    InvestmentController();
    void addAsset(std::shared_ptr<Asset> asset);
    std::vector<std::shared_ptr<Asset>> getAllAssets();
    std::shared_ptr<Asset> getAsset(QString ticker);
    void addAssets(std::vector<Asset>& assets);
    int getAssetQuantity(QString ticker);
    double getAveragePrice(QString ticker);
    double getAssetDistribution(QString ticker);
    double getAssetTotalYield(QString ticker);
    double getAssetCurrentPrice(QString ticker);
};

#endif // INVESTMENTCONTROLLER_H
