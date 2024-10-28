#include "assetwindow.h"
#include "ui_assetwindow.h"
#include "basics.h"

AssetWindow::AssetWindow(Asset *asset, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AssetWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1149, 642);
    this->setMinimumSize(1149, 642);

    // Set asset
    this->asset = asset;

    // Update tables
    updateTransactionTable();
    updateYieldTable();

    // Update labels
    ui->label_ticker->setText(asset->getTicker());

    // Configure label styles
    ui->label_ticker->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_transactions->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_yields->setStyleSheet("color: rgb(255, 255, 255);");

    // Set window background color
    ui->centralwidget->setStyleSheet("background-color: rgb(18, 18, 18);");
}

AssetWindow::~AssetWindow()
{
    delete ui;
}

void AssetWindow::updateTransactionTable()
{
    // Get events
    auto transactions = asset->getTransactions();
    auto reorganizations = asset->getReorganizations();
    auto events = mergeAndSortEvents(transactions, reorganizations);

    // Configure transaction table
    QStringList headerLabels = {"Tipo de operação", "Data da operação", "Quantidade", "Preço unitário", "Total da operação",
                                "Quantidade acumulado", "Preço médio", "Valor total investido"};
    configureTableWidget(headerLabels, ui->tableWidget_transactions);

    // Init variables
    int row = 0;
    int accumulatedQuantity = 0;
    double accumulatedTotal = 0;
    double averagePrice = 0;
    QStringList itens;

    for(auto event: events)
    {
        int style;

        if(event->getEventType() == EventType::REORGANIZATION)
        {
            // Cast to reorganization
            Reorganization *reorganization = dynamic_cast<Reorganization*>(event);

            // Check cast
            if (!reorganization)
            {
                qDebug() << "Reorganization cast error";
                continue;
            }

            // Set style
            style = HIGHLIGHT_CELL;

            ReorganizationType reorganizationType = reorganization->getReorganizationType();

            // Check reorganization type
            if(reorganizationType == ReorganizationType::GRUPAMENTO)
            {
                accumulatedQuantity = (accumulatedQuantity == 0) ? 0 : (accumulatedQuantity / reorganization->getRatio());
            }
            else if (reorganizationType == ReorganizationType::DESDOBRAMENTO)
            {
                accumulatedQuantity = static_cast<int>(accumulatedQuantity * reorganization->getRatio());
            }
            else
            {
                throw std::invalid_argument("Reorganization type invalid");
            }

            // Calculate average price and total
            averagePrice = (accumulatedQuantity > 0) ? (accumulatedTotal / accumulatedQuantity) : 0;

            // Populate string list
            itens = {getReorganizationTypeString(reorganizationType), reorganization->getDate().toString("dd/MM/yyyy"),
                    QString::number(reorganization->getRatio()), "-", "-",
                    QString::number(accumulatedQuantity), "R$ " + QString::number(averagePrice, 'f', 2),
                    "R$ " + QString::number(accumulatedTotal, 'f', 2)};
        }
        else if(event->getEventType() == EventType::TRANSACTION)
        {
            // Cast to transaction
            Transaction *transaction = dynamic_cast<Transaction*>(event);

            // Check cast
            if (!transaction)
            {
                qDebug() << "Transaction cast error";
                continue;
            }

            // Get variables
            TransactionType transactionType = transaction->getTransactionType();
            int quantity = transaction->getQuantity();
            double unitaryPrice = transaction->getUnitaryPrice();
            double totalOperation = quantity * unitaryPrice;
            QDate date = transaction->getDate();

            // Check transaction type
            if(transactionType == TransactionType::COMPRA)
            {
                accumulatedQuantity += quantity;
                accumulatedTotal = (accumulatedQuantity > 0) ? accumulatedTotal + totalOperation : 0;
                averagePrice = (accumulatedQuantity > 0) ? (accumulatedTotal / accumulatedQuantity) : 0;
                style = STANDART_CELL;
            }
            else if (transactionType == TransactionType::VENDA)
            {
                accumulatedQuantity -= quantity;
                accumulatedTotal = accumulatedQuantity * averagePrice;
                averagePrice = (accumulatedQuantity > 0) ? averagePrice : 0;
                style = HIGHLIGHT_CELL;
            }
            else if (transactionType == TransactionType::BONIFICACAO)
            {
                accumulatedQuantity += quantity * (quantity / 100.0);
                averagePrice = (accumulatedQuantity > 0) ? (accumulatedTotal / accumulatedQuantity) : 0;
                accumulatedTotal = accumulatedQuantity * averagePrice;
                style = HIGHLIGHT_CELL;
            }
            else if (transactionType == TransactionType::RESTITUICAO)
            {
                accumulatedTotal -= unitaryPrice;
                averagePrice = (accumulatedQuantity > 0) ? (accumulatedTotal / accumulatedQuantity) : 0;
                style = HIGHLIGHT_CELL;
            }
            else
            {
                throw std::invalid_argument("Transaction type invalid");
            }

            // Populate string list
            itens = {getTransactionTypeString(transactionType), date.toString("dd/MM/yyyy"),
                     QString::number(quantity) + ((transactionType == TransactionType::BONIFICACAO) ? "%" : ""),
                    "R$ " + QString::number(unitaryPrice, 'f', 2), "R$ " + QString::number(totalOperation, 'f', 2),
                    QString::number(accumulatedQuantity), "R$ " + QString::number(averagePrice, 'f', 2),
                    "R$ " + QString::number(accumulatedQuantity * averagePrice, 'f', 2)};
        }
        else
        {
            qDebug() << "Invalid event type";
            continue;
        }

        // Insert itens
        addTableWidgetItens(ui->tableWidget_transactions, row, itens, style);

        // Add row
        ++row;
    }
}

void AssetWindow::updateYieldTable()
{
    // Get Yield
    auto yields = asset->getYields();

    // Configure yield table
    QStringList headerLabels = {"Tipo de rendimento", "Data do rendimento", "Valor recebido", "Valor agregado"};
    configureTableWidget(headerLabels, ui->tableWidget_yields);

    // Init variables
    int row = 0;
    double accumulatedValue = 0;

    for(auto yield: yields)
    {
        // Get variables
        YieldType yieldType = yield.getYieldType();
        double value = yield.getValue();
        accumulatedValue += value;

        // Set style
        int style = (yieldType == YieldType::DIVIDENDO) ? HIGHLIGHT_CELL : STANDART_CELL;

        // Create string list
        QStringList itens = {getYieldTypeString(yieldType), yield.getDate().toString("dd/MM/yyyy"),
                             "R$ " + QString::number(value, 'f', 2), "R$ " + QString::number(accumulatedValue, 'f', 2)};

        // Insert itens
        addTableWidgetItens(ui->tableWidget_yields, row, itens, style);

        // Add row
        ++row;
    }
}


