#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <string>

enum class TransactionType
{
    COMPRA,
    VENDA
};

enum class AssetType
{
    ACAO,
    FUNDO
};

enum class YieldType
{
    DIVIDENDO,
    JCP,
    RENDIMENTO
};

enum class ReorganizationType
{
    GRUPAMENTO,
    DESDOBRAMENTO
};

enum class EventType
{
    TRANSACTION,
    YIELD,
    REORGANIZATION
};

QString getTransactionTypeString(TransactionType type);
QString getAssetTypeString(AssetType type);
QString getYieldTypeString(YieldType type);
QString getReorganizationTypeString(ReorganizationType type);
bool isValidTransactionType(const QString& type);
bool isValidAssetType(const QString& type);
bool isValidYieldType(const QString& type);
bool isValidReorganizationType(const QString& type);

#endif // CONSTANTS_H
