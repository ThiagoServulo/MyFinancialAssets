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

QString Asset::getTicker() const
{
    return ticker;
}

AssetType Asset::getAssetType()
{
    return assetType;
}

double Asset::getCurrentPrice()
{
    return (getQuantity(nullptr, nullptr) == 0) ? 0 : currentPrice;
}

void Asset::setCurrentPrice(double price)
{
    currentPrice = price;
}

int Asset::getQuantity(QDate *init, QDate *end) const
{
    int quantity  = 0;
    auto transactions = getTransactions(init, end);
    auto reorganizations = getReorganizations(init, end);
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
                else if(transaction->getTransactionType() == TransactionType::BONIFICACAO)
                {
                    quantity += quantity * (transaction->getQuantity() / 100.0);
                }
            }
        }
    }
    return quantity;
}

double Asset::getTotalYield(QDate *init, QDate *end)
{
    // Init total
    double total = 0;

    // Get yields
    auto yields = getYields(init, end);

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
    return (getQuantity(nullptr, nullptr) == 0) ? 0 : (getTotalInvested(nullptr, nullptr) / getQuantity(nullptr, nullptr));
}

double Asset::getTotalInvested(QDate *init, QDate *end)
{
    // Return total invested
    return (getQuantity(init, end) == 0) ? 0 :
           (getTransactionsTotal(TransactionType::COMPRA, init, end) -
            getTransactionsTotal(TransactionType::VENDA, init, end) -
            getTransactionsTotal(TransactionType::RESTITUICAO, init, end));
}

double Asset::getProfitPercentage()
{
    // Return profit percentage
    return (getAveragePrice() == 0) ? 0 : ((getCurrentPrice() - getAveragePrice()) / getAveragePrice()) * 100;
}

double Asset::getCapitalGain()
{
    // Return capital gain
    return ((getCurrentPrice() - getAveragePrice()) * getQuantity(nullptr, nullptr)) + getTotalYield(nullptr, nullptr);
}

double Asset::getTransactionsTotal(TransactionType transactionType, QDate *init, QDate *end)
{
    // Init total
    double total = 0;

    // Get transactions
    std::vector<Transaction> transactions = getTransactions(nullptr, nullptr);

    for(auto transaction: transactions)
    {
        // Get quantity
        int quantity = (transaction.getTransactionType() == TransactionType::RESTITUICAO) ?
                    1 : transaction.getQuantity();

        // Check transaction type
        if(transaction.getTransactionType() == transactionType)
        {
            // Check date
            if((init == nullptr && end == nullptr) ||
                    (transaction.getEventDate() >= *init && transaction.getEventDate() < *end))
            {
                total += (transaction.getUnitaryPrice() * quantity);
            }
        }
    }

    // Return total invested
    return total;
}

double Asset::getProfitPercentageTotal()
{
    // Return profit percentage
    return (getCapitalGainTotal() /
            (getTransactionsTotal(TransactionType::COMPRA, nullptr, nullptr) -
             getTransactionsTotal(TransactionType::RESTITUICAO, nullptr, nullptr))) * 100.0;
}

double Asset::getCapitalGainTotal()
{
    // Return capital gain
    return (getTotalYield(nullptr, nullptr) +
            getTransactionsTotal(TransactionType::RESTITUICAO, nullptr, nullptr) +
            getTransactionsTotal(TransactionType::VENDA, nullptr, nullptr) -
            getTransactionsTotal(TransactionType::COMPRA, nullptr, nullptr));
}

