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

QString getOperationTypeString(TransactionType type);
QString getAssetTypeString(AssetType type);
QString getYieldTypeString(YieldType type);
bool isValidOperationType(const QString& type);
bool isValidAssetType(const QString& type);
bool isYieldType(const QString& type);

#endif // CONSTANTS_H
