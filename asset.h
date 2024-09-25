#ifndef ASSET_H
#define ASSET_H

#include "constants.h"
#include "eventcontroller.h"
#include <QString>

class Asset: public EventController
{
private:
    QString ticker;
    AssetType assetType;

public:
    Asset();
    Asset(QString ticker, AssetType assetType);
};

#endif // ASSET_H
