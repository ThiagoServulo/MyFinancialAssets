#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <string>

enum class OperationType
{
    COMPRA,
    VENDA,
    DIVIDENDOS,
    JCP,
    RENDIMENTOS
};

enum class AssetType
{
    ACAO,
    FUNDO
};

QString getOperationTypeString(OperationType type);
QString getAssetTypeString(AssetType type);
bool isValidOperationType(const QString& type);


#endif // CONSTANTS_H
