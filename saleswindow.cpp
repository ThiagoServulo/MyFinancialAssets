#include "saleswindow.h"
#include "ui_saleswindow.h"
#include "basics.h"
#include "database.h"

SalesWindow::SalesWindow(AssetController *assetController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SalesWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1090, 591);
    this->setMinimumSize(1090, 591);

    // Configure label assets
    ui->label_assets->setStyleSheet("color: rgb(255, 255, 255);");

    // Configure stock table
    QStringList headerLabels = {"Ticker", "Data de entrada", "Data de saída", "Rendimento total", "Preço médio de compra",
                                "Preço médio de venda", "Valorização", "Ganho de capital"};
    configureTableWidget(headerLabels, ui->tableWidget_sales);

    // Save asset controller
    this->assetController = assetController;

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

    for(auto asset: this->assetController->getAllAssets())
    {
        if(database.getTickerQuantity(asset->getTicker()) == 0)
        {
            QStringList itens = {asset->getTicker(), "10/10/2020" , "10/09/2024", "R$ " + QString::number(assetController->getAssetTotalYield(asset->getTicker()), 'f', 2),
                                 "R$ 10.00" , "R$ 20.00" , + " 10%", "R$ 120.00" };

            addTableWidgetItens(ui->tableWidget_sales, row, itens, STANDART_CELL);

            ++row;
        }
    }
}

