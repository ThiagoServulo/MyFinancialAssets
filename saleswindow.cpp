#include "saleswindow.h"
#include "ui_saleswindow.h"
#include "basics.h"
#include "database.h"
#include "assetwindow.h"

SalesWindow::SalesWindow(InvestmentController *investmentController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SalesWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1090, 591);
    this->setMinimumSize(1090, 591);

    // Set investiment controller
    this->investmentController = investmentController;

    // Configure label assets
    ui->label_assets->setStyleSheet("color: rgb(255, 255, 255);");

    // Configure stock table
    QStringList headerLabels = {"Ticker", "Data de entrada", "Data de saída", "Rendimento total", "Total em compras",
                                "Total em vendas", "Valorização", "Ganho de capital"};
    configureTableWidget(headerLabels, ui->tableWidget_sales);

    // Update table widget sales
    updateTableWidgetSales();
}

SalesWindow::~SalesWindow()
{
    delete ui;
}

void SalesWindow::updateTableWidgetSales()
{
    // Init rows
    int row = 0;

    for(auto asset: this->investmentController->getAllAssets())
    {
        if(investmentController->getAsset(asset->getTicker())->getQuantity() == 0)
        {
            // Get values
            QDate purchaseDate = asset->getEspecifiedTransactionDate(TransactionType::COMPRA, true);
            QDate limitDate = asset->getEspecifiedTransactionDate(TransactionType::VENDA, false);
            double purchaseTotal = asset->getTransactionsTotal(TransactionType::COMPRA);
            double saleTotal = asset->getTransactionsTotal(TransactionType::VENDA);
            double profitPercentage = asset->getProfitPercentageTotal();
            double capitalGain = asset->getCapitalGainTotal();

            // Get itens
            QStringList itens = {asset->getTicker(), purchaseDate.toString("dd/MM/yyyy"),
                                 limitDate.toString("dd/MM/yyyy"),
                                 "R$ " + QString::number(asset->getTotalYield(), 'f', 2),
                                 "R$ " + QString::number(purchaseTotal, 'f', 2),
                                 "R$ " + QString::number(saleTotal, 'f', 2),
                                 QString::number(profitPercentage, 'f', 2) + "%",
                                 "R$ " + QString::number(capitalGain, 'f', 2)};

            // Insert new row
            addTableWidgetItens(ui->tableWidget_sales, row, itens, STANDART_CELL);
            ++row;
        }

        // TODO: Fazer a linha de total
    }
}

void SalesWindow::on_tableWidget_sales_cellDoubleClicked(int row, int column)
{
    // Get ticker
    QString ticker = ui->tableWidget_sales->item(row , 0)->text();

    // Check ticker
    if(ticker != "Total")
    {
        AssetWindow *assetWindow = new AssetWindow(investmentController->getAsset(ticker).get(), this);
        assetWindow->show();
    }
}

