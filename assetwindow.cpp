#include "assetwindow.h"
#include "ui_assetwindow.h"
#include "database.h"
#include "basics.h"

AssetWindow::AssetWindow(QString ticker, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AssetWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1082, 642);
    this->setMinimumSize(1082, 642);

    // Update transaction table
    updateTransactionTable(ticker);

    // Update yield table
    updateYieldTable(ticker);

    // Update labels
    ui->label_ticker->setText(ticker);

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

void AssetWindow::updateTransactionTable(QString ticker)
{
    Database database;

    // Get transactions
    auto transactions = database.getTickerTransactions(ticker);

    // Configure transaction table
    QStringList headerLabels = {"Operação", "Data da operação", "Quantidade", "Preço unitário", "Total da operação", "Quantidade acumulado", "Preço médio", "Valor total acumulado"};
    configureTableWidget(headerLabels, ui->tableWidget_transactions);

    // Init variables
    int row = 0;
    int accumulatedQuantity = 0;
    double accumulatedTotal = 0;
    double averagePrice = 0;

    for(auto transaction: transactions)
    {
        // Get variables
        TransactionType transactionType = transaction.getTransactionType();
        int quantity = transaction.getQuantity();
        double unitaryPrice = transaction.getUnitaryPrice();
        double totalOperation = quantity * unitaryPrice;
        int style;

        // Check transaction type
        if(transactionType == TransactionType::COMPRA)
        {
            accumulatedQuantity += quantity;
            accumulatedTotal += totalOperation;
            style = STANDART_CELL;
        }
        else if (transactionType == TransactionType::VENDA)
        {
            accumulatedQuantity -= quantity;
            accumulatedTotal -= totalOperation;
            style = HIGHLIGHT_CELL;
        }
        else
        {
            throw std::invalid_argument("Transaction type invalid");
        }

        // Calculate average price
        averagePrice = (accumulatedQuantity > 0) ? (accumulatedTotal / accumulatedQuantity) : 0;
        accumulatedTotal = (accumulatedQuantity > 0) ? accumulatedTotal : 0;

        // Create string list
        QStringList itens = {getTransactionTypeString(transactionType), transaction.getDate().toString("dd/MM/yyyy"), QString::number(quantity),
                            QString::number(unitaryPrice, 'f', 2), QString::number(totalOperation, 'f', 2), QString::number(accumulatedQuantity),
                            QString::number(averagePrice, 'f', 2), QString::number(accumulatedTotal, 'f', 2)};

        // Insert itens
        addTableWidgetItens(ui->tableWidget_transactions, row, itens, style);

        // Add row
        ++row;
    }
}

void AssetWindow::updateYieldTable(QString ticker)
{
    Database database;

    // Get Yield
    auto yields = database.getTickerYields(ticker);

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


