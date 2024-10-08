#include "constants.h"
#include <QHeaderView>

QString getTransactionTypeString(TransactionType type)
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
    return (type == getTransactionTypeString(TransactionType::COMPRA) ||
            type == getTransactionTypeString(TransactionType::VENDA));
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

void configureTableWidget(QStringList headerLabels, QTableWidget *tableWidget)
{
    // Configure table
    tableWidget->setColumnCount(headerLabels.size());
    tableWidget->setHorizontalHeaderLabels(headerLabels);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // Set column width
    for(int i = 0; i < headerLabels.size(); i++)
    {
        QFontMetrics fontMetrics(tableWidget->font());
        int width = fontMetrics.horizontalAdvance(headerLabels[i]) + 40;
        tableWidget->setColumnWidth(i, width);
    }

    // Set table widget style
    tableWidget->setStyleSheet(
        "QTableWidget { background-color: rgb(28, 28, 28); color: rgb(255, 255, 255); }"
        "QHeaderView::section { background-color: rgb(44, 44, 46); color: rgb(255, 255, 255); }"
        "QTableView::item:selected { background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); }"
    );

    // Change focus policy
    tableWidget->setFocusPolicy(Qt::NoFocus);

    // Set header style
    QHeaderView* header = tableWidget->horizontalHeader();
    header->setStyleSheet("QHeaderView::section { font-size: 14px; font-weight: bold; background-color: rgb(28, 28, 28); color: rgb(255, 255, 255); }");
}
