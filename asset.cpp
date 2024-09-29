#include "asset.h"

Asset::Asset()
{

}

Asset::Asset(QString ticker, AssetType assetType)
{
    this->ticker = ticker;
    this->assetType = assetType;
}

QString Asset::getTicker()
{
    return this->ticker;
}

AssetType Asset::getAssetType()
{
    return this->assetType;
}
