#include "asset.h"

Asset::Asset()
{

}

Asset::Asset(QString ticker, AssetType assetType)
{
    this->ticker = ticker;
    this->assetType = assetType;
}
