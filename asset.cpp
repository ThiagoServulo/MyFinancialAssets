#include "asset.h"

Asset::Asset()
{

}

Asset::Asset(QString ticker, AssetType assetType)
{
    this->ticker = ticker;
    this->assetType = assetType;
}
/*
void Asset::addEvent(Event event)
{
    this->eventController.addEvent(event);
}

void Asset::getTransactionList()
{
    for (auto it = this->eventController.begin(); it != eventController.end(); ++it)
    this->eventController;
}*/
