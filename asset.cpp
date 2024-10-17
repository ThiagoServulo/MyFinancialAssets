#include "asset.h"
#include "database.h"
#include "assetapi.h"
#include "basics.h"

Asset::Asset()
{

}

Asset::Asset(QString ticker, AssetType assetType, double currentPrice)
{
    this->ticker = ticker;
    this->assetType = assetType;
    this->currentPrice = currentPrice;
}

QString Asset::getTicker()
{
    return this->ticker;
}

AssetType Asset::getAssetType()
{
    return this->assetType;
}

double Asset::getCurrentPrice()
{
    return this->currentPrice;
}

int Asset::getQuantity()
{
    int quantity  = 0;
    auto transactions = getTransactions();
    auto reorganizations = getReorganizations();
    auto events = mergeAndSortEvents(transactions, reorganizations);

    for(auto event: events)
    {
        if(event->getEventType() == EventType::REORGANIZATION)
        {
            // Cast to reorganization
            Reorganization* reorganization = dynamic_cast<Reorganization*>(event);

            // Check cast
            if (reorganization)
            {
                // Check reorganization type
                if(reorganization->getReorganizationType() == ReorganizationType::GRUPAMENTO)
                {
                    quantity = (quantity == 0) ? 0 : (quantity / reorganization->getRatio());
                }
                else if (reorganization->getReorganizationType() == ReorganizationType::DESDOBRAMENTO)
                {
                    quantity = static_cast<int>(quantity * reorganization->getRatio());
                }
                else
                {
                    throw std::invalid_argument("Reorganization type invalid");
                }
            }
        }
        else if(event->getEventType() == EventType::TRANSACTION)
        {
            // Cast to transaction
            Transaction* transaction = dynamic_cast<Transaction*>(event);

            // Check cast
            if (transaction)
            {
                // Check transaction type
                if(transaction->getTransactionType() == TransactionType::COMPRA)
                {
                    quantity += transaction->getQuantity();
                }
                else if(transaction->getTransactionType() == TransactionType::VENDA)
                {
                    quantity -= transaction->getQuantity();
                }
                else
                {
                    throw std::invalid_argument("Transaction type invalid");
                }
            }
        }
    }
    return quantity;
}

double Asset::getTotalYield()
{
    Database database;
    return database.getTickerTotalYield(this->ticker);
}

double Asset::getAveragePrice()
{
    Database database;
    return database.getTickerAveragePrice(this->ticker);
}
