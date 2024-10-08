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
        // Insert new row
        ui->tableWidget_transactions->insertRow(row);

        // Get variables
        TransactionType transactionType = transaction.getTransactionType();
        int quantity = transaction.getQuantity();
        double unitaryPrice = transaction.getUnitaryPrice();
        double totalOperation = quantity * unitaryPrice;

        // Check transaction type
        if(transactionType == TransactionType::COMPRA)
        {
            accumulatedQuantity += quantity;
            accumulatedTotal += totalOperation;
        }
        else if (transactionType == TransactionType::VENDA)
        {
            accumulatedQuantity -= quantity;
            accumulatedTotal -= totalOperation;
        }
        else
        {
            throw std::invalid_argument("Transaction type invalid");
        }

        // Calculate average price
        averagePrice = (accumulatedQuantity > 0) ? (accumulatedTotal / accumulatedQuantity) : 0;
        accumulatedTotal = (accumulatedQuantity > 0) ? accumulatedTotal : 0;

        // Insert data
        ui->tableWidget_transactions->setItem(row, 0, new QTableWidgetItem(getTransactionTypeString(transactionType)));
        ui->tableWidget_transactions->setItem(row, 1, new QTableWidgetItem(transaction.getDate().toString("dd/MM/yyyy")));
        ui->tableWidget_transactions->setItem(row, 2, new QTableWidgetItem(QString::number(quantity)));
        ui->tableWidget_transactions->setItem(row, 3, new QTableWidgetItem(QString::number(unitaryPrice, 'f', 2)));
        ui->tableWidget_transactions->setItem(row, 4, new QTableWidgetItem(QString::number(totalOperation, 'f', 2)));
        ui->tableWidget_transactions->setItem(row, 5, new QTableWidgetItem(QString::number(accumulatedQuantity)));
        ui->tableWidget_transactions->setItem(row, 6, new QTableWidgetItem(QString::number(averagePrice, 'f', 2)));
        ui->tableWidget_transactions->setItem(row, 7, new QTableWidgetItem(QString::number(accumulatedTotal, 'f', 2)));

        // Add row
        ++row;
    }
}

void changeRowBackgroundColor(QTableWidget *tableWidget, int row, const QColor &color)
{
    int columnCount = tableWidget->columnCount();

    // Itera sobre cada célula da linha
    for (int col = 0; col < columnCount; ++col) {
        QTableWidgetItem *item = tableWidget->item(row, col);

        if (!item) {
            // Se não houver item, cria um novo
            item = new QTableWidgetItem();
            tableWidget->setItem(row, col, item);
        }

        // Define a cor de fundo para a célula
        item->setBackground(color);
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
        // Insert new row
        ui->tableWidget_yields->insertRow(row);

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


