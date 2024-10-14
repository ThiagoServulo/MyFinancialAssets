#ifndef INVESTMENTCONTROLLER_H
#define INVESTMENTCONTROLLER_H

#include "asset.h"

class InvestmentController
{
private:
    std::vector<std::shared_ptr<Asset>> assetList;

public:
    InvestmentController();
    std::shared_ptr<Asset> getAsset(QString ticker);
    void addAssets(std::vector<Asset>& assets);
    std::vector<std::shared_ptr<Asset>> getAllAssets();
    double getAssetDistribution(QString ticker);
};

#endif // INVESTMENTCONTROLLER_H
