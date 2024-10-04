#ifndef ASSETCONTROLLER_H
#define ASSETCONTROLLER_H

#include "asset.h"

class AssetController
{
private:
    std::vector<std::shared_ptr<Asset>> assetList;

public:
    AssetController();
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

#endif // ASSETCONTROLLER_H
