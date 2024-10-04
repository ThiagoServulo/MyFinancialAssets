#include "mainwindow.h"
#include "newtransactionwindow.h"
#include "newyieldwindow.h"
#include "newreorganizationwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"
#include "asset.h"
#include "event.h"
#include "transaction.h"
#include "yield.h"
#include "assetapi.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configure stock table
    QStringList headerLabels = {"Ticker", "Distribuição", "Quantidade", "Rendimento", "Preço médio", "Preço atual", "Valorização", "Ganho de capital"};
    ui->tableWidget_stocks->setColumnCount(headerLabels.size());
    ui->tableWidget_stocks->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget_stocks->verticalHeader()->setVisible(false);
    ui->tableWidget_stocks->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_stocks->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Configure fund table
    ui->tableWidget_funds->setColumnCount(headerLabels.size());
    ui->tableWidget_funds->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget_funds->verticalHeader()->setVisible(false);
    ui->tableWidget_funds->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_funds->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Init asset controller
    if(!database.assetControllerInitialization(&assetController))
    {
        qDebug() << "Erro in asset controller Initialization";
    }

    // Init stocks table
    updateSotckAndFundTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionTransaction_triggered()
{
    NewTransactionWindow *newTransactionWindow = new NewTransactionWindow(this);
    newTransactionWindow->show();
}

void MainWindow::on_actionYield_triggered()
{
    NewYieldWindow *newYieldWindow = new NewYieldWindow(&assetController, this);
    newYieldWindow->show();
}

void MainWindow::on_pushButton_clicked()
{
    AssetApi assetApi;
    double valor = assetApi.getAssetCurrentPrice("BBAS3");
    qDebug() << valor;
}

void MainWindow::on_actionReorganization_triggered()
{
    NewReorganizationWindow *newReorganizationWindow = new NewReorganizationWindow(this);
    newReorganizationWindow->show();
}

void MainWindow::updateSotckAndFundTable()
{
    // Get assets
    std::vector<std::shared_ptr<Asset>> assets = assetController.getAllAssets();

    // Init variables
    int stockRow = 0;
    int fundRow = 0;
    int totalStockQuantity = 0;
    int totalFundQuantity = 0;
    double totalStockYield = 0;
    double totalFundYield = 0;
    double totalFundCapitalGain = 0;
    double totalStockCapitalGain = 0;

    for(auto asset: assets)
    {
        // Init variables
        QString profitPercentage = "-";
        QString capitalGain = "-";
        QString currentPriceStr = "-";

        // Get values
        QString ticker = asset->getTicker();
        int quantity = assetController.getAssetQuantity(ticker);
        double totalYield = assetController.getAssetTotalYield(ticker);
        double averagePrice = assetController.getAveragePrice(ticker);
        double currentPrice = assetController.getAssetCurrentPrice(ticker);

        // Show values if is relevant
        if(currentPrice > 0)
        {
            profitPercentage = QString::number(((currentPrice - averagePrice) / averagePrice) * 100, 'f', 2);
            capitalGain = QString::number(((currentPrice - averagePrice) * quantity) + totalYield, 'f', 2);
            currentPriceStr = QString::number(currentPrice, 'f', 2);
        }

        // Check asset type
        if(asset->getAssetType() == AssetType::ACAO)
        {
            // Add new line
            addNewLineToTable(ui->tableWidget_stocks, stockRow, ticker, QString::number(assetController.getAssetDistribution(ticker), 'f', 2), QString::number(quantity), QString::number(totalYield, 'f', 2), QString::number(averagePrice, 'f', 2), currentPriceStr, profitPercentage, capitalGain);

            // Update variables
            ++stockRow;
            totalStockQuantity += quantity;
            totalStockYield += totalYield;
            totalStockCapitalGain += (currentPriceStr == "-") ? 0 : std::stod(capitalGain.toUtf8().constData());
        }
        else if(asset->getAssetType() == AssetType::FUNDO)
        {
            // Add new line
            addNewLineToTable(ui->tableWidget_funds, fundRow, ticker, QString::number(assetController.getAssetDistribution(ticker), 'f', 2), QString::number(quantity), QString::number(totalYield, 'f', 2), QString::number(averagePrice, 'f', 2), currentPriceStr, profitPercentage, capitalGain);

            // Update variables
            ++fundRow;
            totalFundQuantity += quantity;
            totalFundYield += totalYield;
            totalFundCapitalGain += (currentPriceStr == "-") ? 0 : std::stod(capitalGain.toUtf8().constData());
        }
        else
        {
            throw std::invalid_argument("Asset type invalid");
        }
    }

    // Add last line
    addNewLineToTable(ui->tableWidget_funds, fundRow, "Total", "100", QString::number(totalFundQuantity), QString::number(totalFundYield, 'f', 2), "-", "-", "-", (totalFundCapitalGain == 0) ? "-" : QString::number(totalFundCapitalGain, 'f', 2));
    addNewLineToTable(ui->tableWidget_stocks, stockRow, "Total", "100", QString::number(totalStockQuantity), QString::number(totalStockYield, 'f', 2), "-", "-", "-", (totalStockCapitalGain == 0) ? "-" : QString::number(totalStockCapitalGain, 'f', 2));
}

void MainWindow::on_tableWidget_stocks_cellDoubleClicked(int row, int column)
{
    qDebug() << "aaaaa " << row;
}

void MainWindow::addNewLineToTable(QTableWidget *tableWidget, int row, QString ticker, QString distribution, QString quantity, QString totalYield, QString averagePrice, QString currentPriceStr, QString profitPercentage, QString capitalGain)
{
    // Insert new row
    tableWidget->insertRow(row);

    // Insert data
    tableWidget->setItem(row, 0, new QTableWidgetItem(ticker));
    tableWidget->setItem(row, 1, new QTableWidgetItem(distribution + "%"));
    tableWidget->setItem(row, 2, new QTableWidgetItem(quantity));
    tableWidget->setItem(row, 3, new QTableWidgetItem(totalYield));
    tableWidget->setItem(row, 4, new QTableWidgetItem(averagePrice));
    tableWidget->setItem(row, 5, new QTableWidgetItem(currentPriceStr));
    tableWidget->setItem(row, 6, new QTableWidgetItem(profitPercentage));
    tableWidget->setItem(row, 7, new QTableWidgetItem(capitalGain));
}
