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
    QStringList headerLabels = {"Ticker", "Data de entrada", "Data de saída", "Rendimento total", "Preço médio de compra",
                                "Preço médio de venda", "Valorização", "Ganho de capital"};
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
    Database database;
    int row = 0;

    // TODO: Implementar a tabela papeis vendidos

    for(auto asset: this->investmentController->getAllAssets())
    {
        if(investmentController->getAsset(asset->getTicker())->getQuantity() == 0)
        {
            auto transactions = asset->getTransactions();
            QDate purchaseDate = getEarliestTransactionDate(transactions, TransactionType::COMPRA);
            QStringList itens = {asset->getTicker(), purchaseDate.toString("dd/MM/yyyy") , "10/09/2024", "R$ " +
                                 QString::number(asset->getTotalYield(), 'f', 2),
                                 "R$ 10.00" , "R$ 20.00" , + " 10%", "R$ 120.00" };

            addTableWidgetItens(ui->tableWidget_sales, row, itens, STANDART_CELL);

            ++row;
        }
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

