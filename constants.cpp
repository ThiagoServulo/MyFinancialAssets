#include "constants.h"

QString getOperationTypeString(OperationType type)
{
    switch (type)
    {
        case OperationType::COMPRA: return "Compra";
        case OperationType::VENDA: return "Venda";
        case OperationType::DIVIDENDOS: return "Dividendos";
        case OperationType::JCP: return "JCP";
        case OperationType::RENDIMENTOS: return "Rendimentos";
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
    return (type == getOperationTypeString(OperationType::COMPRA) ||
            type == getOperationTypeString(OperationType::VENDA) ||
            type == getOperationTypeString(OperationType::DIVIDENDOS) ||
            type == getOperationTypeString(OperationType::JCP) ||
            type == getOperationTypeString(OperationType::RENDIMENTOS));
}
