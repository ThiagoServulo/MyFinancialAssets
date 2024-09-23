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

QString getYieldTypeString(YieldType type)
{
    switch (type)
    {
        case YieldType::RENDIMENTO: return "Rendimento";
        case YieldType::DIVIDENDO: return "Dividendo";
        case YieldType::JCP: return "JCP";
        default: return "Desconhecido";
    }
}

QString getReorganizationTypeString(ReorganizationType type)
{
    switch (type)
    {
        case ReorganizationType::DESDOBRAMENTO: return "Desdobramento";
        case ReorganizationType::GRUPAMENTO: return "Grupamento";
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

bool isValidYieldType(const QString& type)
{
    return (type == getYieldTypeString(YieldType::RENDIMENTO) ||
            type == getYieldTypeString(YieldType::DIVIDENDO) ||
            type == getYieldTypeString(YieldType::JCP));
}

bool isValidReorganizationType(const QString& type)
{
    return (type == getReorganizationTypeString(ReorganizationType::DESDOBRAMENTO) ||
            type == getReorganizationTypeString(ReorganizationType::GRUPAMENTO));
}
