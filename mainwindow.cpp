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
#include "assetwindow.h"
#include "basics.h"
#include "saleswindow.h"
#include "newfixedincomewindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1010, 600);
    this->setMinimumSize(1010, 600);

    // Create database
    database.prepareDatabase();

    // Configure stock and fund tables
    QStringList headerLabels = {"Ticker", "Distribuição", "Quantidade", "Rendimento", "Preço médio", "Preço atual", "Valorização", "Ganho de capital"};
    configureTableWidget(headerLabels, ui->tableWidget_stocks);
    configureTableWidget(headerLabels, ui->tableWidget_funds);

    headerLabels = {"Data da compra", "Descrição do investimento", "Rendimento esperado", "Valor investido", "Valor atual", "Rendimento", "Data limite"};
    configureTableWidget(headerLabels, ui->tableWidget_fixedIncome);

    // Init asset controller
    if(!database.investmentcontrollerInitialization(&investmentcontroller))
    {
        qDebug() << "Erro in asset controller Initialization";
    }

    // Init stocks table
    updateSotckAndFundTable();

    // Set main window background color
    this->setStyleSheet("QMainWindow { background-color: rgb(18, 18, 18); }");

    // Set tab background color
    ui->tab_funds->setStyleSheet("background-color: rgb(18, 18, 18);");
    ui->tab_stocks->setStyleSheet("background-color: rgb(18, 18, 18);");
    ui->tab_general->setStyleSheet("background-color: rgb(18, 18, 18);");
    ui->tab_fixedIncome->setStyleSheet("background-color: rgb(18, 18, 18);");

    // Set check boxes style
    ui->checkBox_hideAssets->setStyleSheet("background-color: rgb(18, 18, 18); color: rgb(255, 255, 255);");
    ui->checkBox_hideFounds->setStyleSheet("background-color: rgb(18, 18, 18); color: rgb(255, 255, 255);");

    // Set tab widget style sheet
    ui->tabWidget->setStyleSheet(
        "QTabWidget::pane { background-color: rgb(18, 18, 18); }"
        "QTabBar::tab { background-color: rgb(28, 28, 28); color: rgb(255, 255, 255); font-size: 14px; font-weight: bold; }"
        "QTabBar::tab:selected { background-color: rgb(18, 18, 18); color: rgb(255, 255, 255); font-size: 14px; font-weight: bold; }"
    );

    // Set initial tab
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionTransaction_triggered()
{
    NewTransactionWindow *newTransactionWindow = new NewTransactionWindow(this);
    newTransactionWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newTransactionWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
    newTransactionWindow->show();
}

void MainWindow::on_actionYield_triggered()
{
    NewYieldWindow *newYieldWindow = new NewYieldWindow(&investmentcontroller, this);
    newYieldWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newYieldWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
    newYieldWindow->show();
}

void MainWindow::on_actionReorganization_triggered()
{
    NewReorganizationWindow *newReorganizationWindow = new NewReorganizationWindow(&investmentcontroller, this);
    newReorganizationWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newReorganizationWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
    newReorganizationWindow->show();
}

void MainWindow::updateSotckAndFundTable()
{
    // Get assets
    std::vector<std::shared_ptr<Asset>> assets = investmentcontroller.getAllAssets();

    // Clear tables
    ui->tableWidget_stocks->clearContents();
    ui->tableWidget_funds->clearContents();
    ui->tableWidget_stocks->setRowCount(0);
    ui->tableWidget_funds->setRowCount(0);

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
        int quantity = asset->getQuantity();
        double totalYield = asset->getTotalYield();
        double averagePrice = (quantity != 0) ? asset->getAveragePrice() : 0;
        double currentPrice = (quantity != 0) ? asset->getCurrentPrice() : 0;

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
            if(!ui->checkBox_hideAssets->isChecked() || (ui->checkBox_hideAssets->isChecked() && quantity != 0))
            {
                addNewLineToTable(ui->tableWidget_stocks, stockRow, ticker, QString::number(investmentcontroller.getAssetDistribution(ticker), 'f', 2),
                                  QString::number(quantity), QString::number(totalYield, 'f', 2), QString::number(averagePrice, 'f', 2),
                                  currentPriceStr, profitPercentage, capitalGain);
                ++stockRow;
            }

            // Update variables
            totalStockQuantity += quantity;
            totalStockYield += totalYield;
            totalStockCapitalGain += (currentPriceStr == "-") ? 0 : std::stod(capitalGain.toUtf8().constData());
        }
        else if(asset->getAssetType() == AssetType::FUNDO)
        {
            // Add new line
            if(!ui->checkBox_hideFounds->isChecked() || (ui->checkBox_hideFounds->isChecked() && quantity != 0))
            {
                addNewLineToTable(ui->tableWidget_funds, fundRow, ticker, QString::number(investmentcontroller.getAssetDistribution(ticker), 'f', 2),
                                  QString::number(quantity), QString::number(totalYield, 'f', 2), QString::number(averagePrice, 'f', 2),
                                  currentPriceStr, profitPercentage, capitalGain);
                ++fundRow;
            }

            // Update variables 
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
    // Get ticker
    QString ticker = ui->tableWidget_stocks->item(row , 0)->text();

    // Check ticker
    if(ticker != "Total")
    {
        AssetWindow *assetWindow = new AssetWindow(ticker, this);
        assetWindow->show();
    }
}

void MainWindow::addNewLineToTable(QTableWidget *tableWidget, int row, QString ticker, QString distribution, QString quantity, QString totalYield, QString averagePrice, QString currentPriceStr, QString profitPercentage, QString capitalGain)
{
    // Set style
    int style = (ticker == "Total") ? (HIGHLIGHT_CELL | FONT_BOLD | FONT_SIZE) : STANDART_CELL;

    // Get itens
    QStringList itens = {ticker, distribution + "%", quantity, "R$ " + totalYield, "R$ " + averagePrice, "R$ " + currentPriceStr,
                         profitPercentage + "%", "R$ " + capitalGain};

    // Insert itens
    addTableWidgetItens(tableWidget, row, itens, style);
}

void MainWindow::on_tableWidget_funds_cellDoubleClicked(int row, int column)
{
    // Get ticker
    QString ticker = ui->tableWidget_funds->item(row , 0)->text();

    // Check ticker
    if(ticker != "Total")
    {
        AssetWindow *assetWindow = new AssetWindow(ticker, this);
        assetWindow->show();
    }
}

void MainWindow::on_actionSales_triggered()
{
    SalesWindow *salesWindow = new SalesWindow(&investmentcontroller, this);
    salesWindow->show();
}

void MainWindow::on_checkBox_hideAssets_stateChanged(int arg1)
{
    updateSotckAndFundTable();
}

void MainWindow::on_actionFixedIncome_triggered()
{
    NewFixedIncomeWindow *newFixedIncomeWindow = new NewFixedIncomeWindow(this);
    newFixedIncomeWindow->setAttribute(Qt::WA_DeleteOnClose);
    //connect(newFixedIncomeWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
    newFixedIncomeWindow->show();
}

