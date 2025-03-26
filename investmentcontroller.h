#ifndef INVESTMENTCONTROLLER_H
#define INVESTMENTCONTROLLER_H

#include "asset.h"
#include "fixedincome.h"
#include "financialinstitution.h"

class InvestmentController
{
private:
    std::vector<std::shared_ptr<Asset>> assets;
    std::vector<std::shared_ptr<FixedIncome>> fixedIncomes;
    std::vector<std::shared_ptr<FinancialInstitution>> financialInstitutions;

public:
    InvestmentController();
    std::shared_ptr<Asset> getAsset(QString ticker);
    void addAsset(std::shared_ptr<Asset> asset);
    int getTotalQuantityOfAssets(AssetType *assetType, QDate *init, QDate *end);
    double getTotalYieldOfAssets(AssetType *assetType, QDate *init, QDate *end);
    double getTotalInvestedOfAssets(AssetType *assetType, QDate *init, QDate *end);
    std::vector<std::shared_ptr<Asset>> getAllAssets();
    double getTotalCapitalGainOfAssets(AssetType assetType);
    double getAssetDistribution(QString ticker);
    void addFixedIncome(std::shared_ptr<FixedIncome> fixedIncome);
    std::vector<std::shared_ptr<FixedIncome>> getFixedIncomes();
    std::vector<std::shared_ptr<FixedIncome>> getSoldFixedIncomes(QDate *limitDate);
    std::vector<std::shared_ptr<FixedIncome>> getPurchasedFixedIncomes(QDate *initDate);
    std::shared_ptr<FixedIncome> getFixedIncome(QDate purchaseDate, QString description);
    double getFixedIncomeTotalInvested(bool status);
    double getFixedIncomeCurrentTotal(bool status);
    double getFixedIncomeTotalYield(bool status);
    double getFixedIncomeTotalYieldPercentage(bool status);
    double getTotalCapitalGainOfAssetsSold();
    double getTotalYieldOfAssetsSold();
    double getTransactionsTotalOfAssetsSold(TransactionType transactionType);
    double getProfitPercentageTotalOfAssetsSold();
    void addFinancialInstitution(std::shared_ptr<FinancialInstitution> financialInstitution);
    std::vector<std::shared_ptr<FinancialInstitution>> getFinancialInstitutions();
    std::shared_ptr<FinancialInstitution> getFinancialInstitution(QString name);
    void removeTickerTransaction(QString ticker, Transaction transaction);
    void removeTickerYield(QString ticker, Yield yield);
    void removeAsset(QString ticker);
    bool deleteFixedIncome(FixedIncome* fixedIncome);
};

#endif // INVESTMENTCONTROLLER_H
