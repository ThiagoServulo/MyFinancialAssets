#include "constants.h"
#include <QHeaderView>

QString getTransactionTypeString(TransactionType type)
{
    switch (type)
    {
        case TransactionType::COMPRA: return "Compra";
        case TransactionType::VENDA: return "Venda";
        case TransactionType::RESTITUICAO: return "Restitução de capital";
        case TransactionType::BONIFICACAO: return "Bonificação de ações";
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

TransactionType getTransactionTypeFromString(QString type)
{
    if (type == getTransactionTypeString(TransactionType::COMPRA))
    {
        return TransactionType::COMPRA;
    }
    else if (type == getTransactionTypeString(TransactionType::VENDA))
    {
        return TransactionType::VENDA;
    }
    else if (type == getTransactionTypeString(TransactionType::RESTITUICAO))
    {
        return TransactionType::RESTITUICAO;
    }
    else if (type == getTransactionTypeString(TransactionType::BONIFICACAO))
    {
        return TransactionType::BONIFICACAO;
    }
    else
    {
        throw std::invalid_argument("Transaction type invalid: " + type.toStdString());
    }
}

AssetType getAssetTypeFromString(QString type)
{
    if (type == getAssetTypeString(AssetType::ACAO))
    {
        return AssetType::ACAO;
    }
    else if (type == getAssetTypeString(AssetType::FUNDO))
    {
        return AssetType::FUNDO;
    }
    else
    {
        throw std::invalid_argument("Asset type invalid: " + type.toStdString());
    }
}

ReorganizationType getReorganizationTypeFromString(QString type)
{
    if (type == getReorganizationTypeString(ReorganizationType::DESDOBRAMENTO))
    {
        return ReorganizationType::DESDOBRAMENTO;
    }
    else if (type == getReorganizationTypeString(ReorganizationType::GRUPAMENTO))
    {
        return ReorganizationType::GRUPAMENTO;
    }
    else
    {
        throw std::invalid_argument("Reorganization type invalid: " + type.toStdString());
    }
}

YieldType getYieldTypeFromString(QString type)
{
    if (type == getYieldTypeString(YieldType::DIVIDENDO))
    {
        return YieldType::DIVIDENDO;
    }
    else if (type == getYieldTypeString(YieldType::JCP))
    {
        return YieldType::JCP;
    }
    else if (type == getYieldTypeString(YieldType::RENDIMENTO))
    {
        return YieldType::RENDIMENTO;
    }
    else
    {
        throw std::invalid_argument("Yield type invalid: " + type.toStdString());
    }
}

bool isValidTransactionType(const QString& type)
{
    return (type == getTransactionTypeString(TransactionType::COMPRA)      ||
            type == getTransactionTypeString(TransactionType::VENDA)       ||
            type == getTransactionTypeString(TransactionType::RESTITUICAO) ||
            type == getTransactionTypeString(TransactionType::BONIFICACAO));
}

bool isValidAssetType(const QString& type)
{
    return (type == getAssetTypeString(AssetType::ACAO) ||
            type == getAssetTypeString(AssetType::FUNDO));
}

bool isValidYieldType(const QString& type)
{
    return (type == getYieldTypeString(YieldType::RENDIMENTO) ||
            type == getYieldTypeString(YieldType::DIVIDENDO)  ||
            type == getYieldTypeString(YieldType::JCP));
}

bool isValidReorganizationType(const QString& type)
{
    return (type == getReorganizationTypeString(ReorganizationType::DESDOBRAMENTO) ||
            type == getReorganizationTypeString(ReorganizationType::GRUPAMENTO));
}
