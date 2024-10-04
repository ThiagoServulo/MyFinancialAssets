#include "assetwindow.h"
#include "ui_assetwindow.h"
#include "database.h"

AssetWindow::AssetWindow(QString ticker, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AssetWindow)
{
    ui->setupUi(this);

    // Update transaction table
    updateTransactionTable(ticker);

    // Update label
    ui->label_ticker->setText(ticker);
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

    // Configure stock table
    QStringList headerLabels = {"Operação", "Data", "Quantidade", "Preço unitário", "Valor total da operação", "Quantidade acumulado", "Preço médio", "Valor total acumulado"};
    ui->tableWidget_transactions->setColumnCount(headerLabels.size());
    ui->tableWidget_transactions->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget_transactions->verticalHeader()->setVisible(false);
    ui->tableWidget_transactions->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_transactions->setEditTriggers(QAbstractItemView::NoEditTriggers);

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
        ui->tableWidget_transactions->setItem(row, 1, new QTableWidgetItem(transaction.getDate().toString()));
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
