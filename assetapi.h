#ifndef ASSETAPI_H
#define ASSETAPI_H

#include <QString>

class AssetApi
{
public:
    static const int API_ERROR = 0;
    AssetApi();
    double getAssetCurrentPrice(QString ticker);

private:
    QString apiKey;
};

#endif // ASSETAPI_H
