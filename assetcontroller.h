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
    void addAssets(std::vector<Asset>& assets);
};

#endif // ASSETCONTROLLER_H
