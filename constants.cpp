#include "constants.h"

QString getOperationTypeString(TransactionType type)
{
    switch (type)
    {
        case TransactionType::COMPRA: return "Compra";
        case TransactionType::VENDA: return "Venda";
        default: return "Desconhecido";
    }
}

QString getAssetTypeString(AssetType type)
{
    switch (type)
    {
        case AssetType::ACAO: return "Ação";
        case AssetType::FUNDO: return "Fundo";
        default: return "Desconhecido";
    }
}

bool isValidOperationType(const QString& type)
{
    return (type == getOperationTypeString(TransactionType::COMPRA) ||
            type == getOperationTypeString(TransactionType::VENDA));
}

bool isValidAssetType(const QString& type)
{
    return (type == getAssetTypeString(AssetType::ACAO) ||
            type == getAssetTypeString(AssetType::FUNDO));
}
