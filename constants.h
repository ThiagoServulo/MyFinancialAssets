#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

enum class TransactionType
{
    COMPRA,
    VENDA,
    RESTITUICAO,
    BONIFICACAO
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
    RENDIMENTO,
    REST_CAPITAL
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
TransactionType getTransactionTypeFromString(QString type);
AssetType getAssetTypeFromString(QString type);
YieldType getYieldTypeFromString(QString type);
ReorganizationType getReorganizationTypeFromString(QString type);
bool isValidTransactionType(const QString& type);
bool isValidAssetType(const QString& type);
bool isValidYieldType(const QString& type);
bool isValidReorganizationType(const QString& type);

#endif // CONSTANTS_H
