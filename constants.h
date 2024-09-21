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

QString getOperationTypeString(TransactionType type);
QString getAssetTypeString(AssetType type);
bool isValidOperationType(const QString& type);
bool isValidAssetType(const QString& type);


#endif // CONSTANTS_H
