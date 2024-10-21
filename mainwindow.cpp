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
#include "updatefixedincomewindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1082, 600);
    this->setMinimumSize(1082, 600);

    // Create database
    database.prepareDatabase();

    // Configure stock and fund tables
    QStringList headerLabels = {"Ticker", "Distribuição", "Quantidade", "Total investido", "Rendimento", "Preço médio", "Preço atual",
                                "Valorização", "Ganho de capital"};
    configureTableWidget(headerLabels, ui->tableWidget_stocks);
    configureTableWidget(headerLabels, ui->tableWidget_funds);

    headerLabels = {"Data da compra", "Descrição do investimento", "Rendimento esperado", "Valor investido",
                    "Valor atual", "Rendimento", "Data limite"};
    configureTableWidget(headerLabels, ui->tableWidget_fixedIncome);

    // Init asset controller
    if(!database.investmentControllerInitialization(&investmentController))
    {
        qDebug() << "Erro in asset controller Initialization";
    }

    // Init tables
    updateSotckAndFundTable();
    updateFixedIncomeTable();

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
    NewTransactionWindow *newTransactionWindow = new NewTransactionWindow(&investmentController, this);
    newTransactionWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newTransactionWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
    newTransactionWindow->show();
}

void MainWindow::on_actionYield_triggered()
{
    NewYieldWindow *newYieldWindow = new NewYieldWindow(&investmentController, this);
    newYieldWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newYieldWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
    newYieldWindow->show();
}

void MainWindow::on_actionReorganization_triggered()
{
    NewReorganizationWindow *newReorganizationWindow = new NewReorganizationWindow(&investmentController, this);
    newReorganizationWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newReorganizationWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
    newReorganizationWindow->show();
}

void MainWindow::updateSotckAndFundTable()
{
    // Get assets
    std::vector<std::shared_ptr<Asset>> assets = investmentController.getAllAssets();

    // Clear tables
    ui->tableWidget_stocks->clearContents();
    ui->tableWidget_funds->clearContents();
    ui->tableWidget_stocks->setRowCount(0);
    ui->tableWidget_funds->setRowCount(0);

    // Init variables
    QStringList itens;
    int stockRow = 0;
    int fundRow = 0;
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
        double averagePrice = asset->getAveragePrice();
        double currentPrice = asset->getCurrentPrice();
        double totalInvested = asset->getTotalInvested();

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
                itens = {ticker, QString::number(investmentController.getAssetDistribution(ticker), 'f', 2) + "%", QString::number(quantity),
                        "R$ " + QString::number(totalInvested, 'f', 2),
                        "R$ " + QString::number(totalYield, 'f', 2), "R$ " + QString::number(averagePrice, 'f', 2),
                        "R$ " + currentPriceStr, profitPercentage + "%", "R$ " + capitalGain};
                addTableWidgetItens(ui->tableWidget_stocks, stockRow, itens, STANDART_CELL);
                ++stockRow;
            }

            // Update variables
            //totalStockQuantity += quantity;
            //totalStockYield += totalYield;
            totalStockCapitalGain += (currentPriceStr == "-") ? 0 : std::stod(capitalGain.toUtf8().constData());
        }
        else if(asset->getAssetType() == AssetType::FUNDO)
        {
            // Add new line
            if(!ui->checkBox_hideFounds->isChecked() || (ui->checkBox_hideFounds->isChecked() && quantity != 0))
            {
                itens = {ticker, QString::number(investmentController.getAssetDistribution(ticker), 'f', 2) + "%", QString::number(quantity),
                         "R$ " + QString::number(totalInvested, 'f', 2),
                         "R$ " + QString::number(totalYield, 'f', 2), "R$ " + QString::number(averagePrice, 'f', 2),
                         "R$ " + currentPriceStr, profitPercentage + "%", "R$ " + capitalGain};
                addTableWidgetItens(ui->tableWidget_funds, fundRow, itens, STANDART_CELL);

                ++fundRow;
            }

            // Update variables 
            //totalFundQuantity += quantity;
            //totalFundYield += totalYield;
            totalFundCapitalGain += (currentPriceStr == "-") ? 0 : std::stod(capitalGain.toUtf8().constData());
        }
        else
        {
            throw std::invalid_argument("Asset type invalid");
        }
    }

    // Add total line
    int totalQuantity = investmentController.getTotalQuantityOfAssets(AssetType::FUNDO);
    double totalYield = investmentController.getTotalYieldOfAssets(AssetType::FUNDO);
    double totalInvested = investmentController.getTotalInvestedOfAssets(AssetType::FUNDO);
    itens = {"Total", "100%", QString::number(totalQuantity), "R$ " + QString::number(totalInvested, 'f', 2),
             "R$ " + QString::number(totalYield, 'f', 2),
            "-", "-", "-",
            (totalFundCapitalGain == 0) ? "-" : "R$ " + QString::number(totalFundCapitalGain, 'f', 2)};
    addTableWidgetItens(ui->tableWidget_funds, fundRow, itens, (HIGHLIGHT_CELL | FONT_BOLD | FONT_SIZE));

    totalQuantity = investmentController.getTotalQuantityOfAssets(AssetType::ACAO);
    totalYield = investmentController.getTotalYieldOfAssets(AssetType::ACAO);
    totalInvested = investmentController.getTotalInvestedOfAssets(AssetType::ACAO);
    itens = {"Total", "100%", QString::number(totalQuantity), "R$ " + QString::number(totalInvested, 'f', 2),
             "R$ " + QString::number(totalYield, 'f', 2), "-", "-", "-",
            (totalStockCapitalGain == 0) ? "-" : "R$ " + QString::number(totalStockCapitalGain, 'f', 2)};
    addTableWidgetItens(ui->tableWidget_stocks, stockRow, itens, (HIGHLIGHT_CELL | FONT_BOLD | FONT_SIZE));
}

void MainWindow::on_tableWidget_stocks_cellDoubleClicked(int row, int column)
{
    // Get ticker
    QString ticker = ui->tableWidget_stocks->item(row , 0)->text();

    // Check ticker
    if(ticker != "Total")
    {
        AssetWindow *assetWindow = new AssetWindow(investmentController.getAsset(ticker).get(), this);
        assetWindow->show();
    }
}

void MainWindow::on_tableWidget_funds_cellDoubleClicked(int row, int column)
{
    // Get ticker
    QString ticker = ui->tableWidget_funds->item(row , 0)->text();

    // Check ticker
    if(ticker != "Total")
    {
        AssetWindow *assetWindow = new AssetWindow(investmentController.getAsset(ticker).get(), this);
        assetWindow->show();
    }
}

void MainWindow::on_actionSales_triggered()
{
    SalesWindow *salesWindow = new SalesWindow(&investmentController, this);
    salesWindow->show();
}

void MainWindow::on_checkBox_hideAssets_stateChanged(int arg1)
{
    updateSotckAndFundTable();
}

void MainWindow::on_actionFixedIncome_triggered()
{
    NewFixedIncomeWindow *newFixedIncomeWindow = new NewFixedIncomeWindow(&investmentController, this);
    newFixedIncomeWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newFixedIncomeWindow, &QObject::destroyed, this, &MainWindow::updateFixedIncomeTable);
    newFixedIncomeWindow->show();
}

void MainWindow::updateFixedIncomeTable()
{
    // Set fixed incomes
    auto fixedIncomes = investmentController.getFixedIncomes(true);

    // Clear table
    ui->tableWidget_fixedIncome->clearContents();
    ui->tableWidget_fixedIncome->setRowCount(0);

    // Set variables
    QStringList itens;
    int row = 0;
    double totalInvested = 0;
    double totalCurrent = 0;

    for(auto fixedIncome: fixedIncomes)
    {
        if(fixedIncome->getStatus() == FixedIncome::VALID)
        {
            // Set itens
            itens = {fixedIncome->getPurchaseDate().toString("dd/MM/yyyy"), fixedIncome->getDescription(), fixedIncome->getYieldExpected(),
                    "R$ " + QString::number(fixedIncome->getInvestedValue(), 'f', 2), "R$ " +
                    QString::number(fixedIncome->getCurrentValue(), 'f', 2), "R$ " +
                    QString::number(fixedIncome->getYield(), 'f', 2), fixedIncome->getLimitDate().toString("dd/MM/yyyy")};

            // Set status
            int style = (fixedIncome->getLimitDate() < QDate::currentDate()) ? HIGHLIGHT_CELL : STANDART_CELL;

            // Insert itens
            addTableWidgetItens(ui->tableWidget_fixedIncome, row, itens, style);

            // Update variables
            totalInvested += fixedIncome->getInvestedValue();
            totalCurrent += fixedIncome->getCurrentValue();
            ++row;
        }
    }

    // Set itens
    itens = {"-", "Total", "-", "R$ " + QString::number(totalInvested, 'f', 2), "R$ " + QString::number(totalCurrent, 'f', 2),
            "R$ " + QString::number(totalCurrent - totalInvested, 'f', 2),
            QString::number(((totalCurrent - totalInvested) / totalCurrent) * 100, 'f', 2) + "%"};

    // Insert itens
    addTableWidgetItens(ui->tableWidget_fixedIncome, row, itens, (HIGHLIGHT_CELL | FONT_BOLD | FONT_SIZE));
}

void MainWindow::on_tableWidget_fixedIncome_cellDoubleClicked(int row, int column)
{
    // Get fixed income informations
    QString description = ui->tableWidget_fixedIncome->item(row , 1)->text();
    QDate purchaseDate =  QDate::fromString(ui->tableWidget_fixedIncome->item(row , 0)->text(), "dd/MM/yyyy");

    // Check description
    if(description != "Total")
    {
        UpdateFixedIncomeWindow *fixedIncomeWindow =
                new UpdateFixedIncomeWindow(investmentController.getFixedIncome(purchaseDate, description).get(), this);
        fixedIncomeWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(fixedIncomeWindow, &QObject::destroyed, this, &MainWindow::updateFixedIncomeTable);
        fixedIncomeWindow->show();
    }
}

void MainWindow::on_checkBox_hideFounds_stateChanged(int arg1)
{
    updateSotckAndFundTable();
}

void MainWindow::on_actionRenda_fixa_encerradas_triggered()
{
    // TODO: Implementar uma nova janela pra mostrar as renda fixas encerradas
}

