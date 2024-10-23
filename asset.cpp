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
    return ticker;
}

AssetType Asset::getAssetType()
{
    return assetType;
}

double Asset::getCurrentPrice()
{
    return (getQuantity() == 0) ? 0 : currentPrice;
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
    // Init total
    double total = 0;

    // Get yields
    auto yields = getYields();

    for(auto yield: yields)
    {
        // Increase total
        total += yield.getValue();
    }

    // Return total
    return total;
}

double Asset::getAveragePrice()
{
    // Calculate average price
    return (getQuantity() == 0) ? 0 : (getTotalInvested() / getQuantity());
}

double Asset::getTotalInvested()
{
    // Return total invested
    return (getQuantity() == 0) ? 0 :
           (getTransactionsTotal(TransactionType::COMPRA) - getTransactionsTotal(TransactionType::VENDA));
}

double Asset::getProfitPercentage()
{
    // Return profit percentage
    return ((getCurrentPrice() - getAveragePrice()) / getAveragePrice()) * 100;
}

double Asset::getCapitalGain()
{
    // Return capital gain
    return ((getCurrentPrice() - getAveragePrice()) * getQuantity()) + getTotalYield();
}

double Asset::getTransactionsTotal(TransactionType transactionType)
{
    // Init total
    double total = 0;

    // Get transactions
    std::vector<Transaction> transactions = getTransactions();

    for(auto transaction: transactions)
    {
        // Check transaction type
        if(transaction.getTransactionType() == transactionType)
        {
            total += (transaction.getUnitaryPrice() * transaction.getQuantity());
        }
    }

    // Return total invested
    return total;
}

double Asset::getProfitPercentageTotal()
{
    // Return profit percentage
    return ((getTransactionsTotal(TransactionType::VENDA) - getTransactionsTotal(TransactionType::COMPRA)) /
            getTransactionsTotal(TransactionType::COMPRA)) * 100;
}

double Asset::getCapitalGainTotal()
{
    // Return capital gain
    return (getTransactionsTotal(TransactionType::VENDA) - getTransactionsTotal(TransactionType::COMPRA)) +
            getTotalYield();
}
