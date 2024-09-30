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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList headerLabels = {"Ticker", "Distribuição", "Quantidade", "Rendimento", "Preço médio", "Preço atual", "Valorização", "Ganho de capital"};
    ui->tableWidget_stocks->setColumnCount(headerLabels.size());
    ui->tableWidget_stocks->setHorizontalHeaderLabels(headerLabels);

    if(!database.assetControllerInitialization(&assetController))
    {
        qDebug() << "Erro in asset controller Initialization";
    }

    // Init stocks table
    updateSotcksTable();
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
    //db.prepareDatabase();

    Yield yield(QDate(2020,5,5), YieldType::DIVIDENDO, 2.55);

    qDebug() << database.insertYield("sapo", yield);
}

void MainWindow::on_actionReorganization_triggered()
{
    NewReorganizationWindow *newReorganizationWindow = new NewReorganizationWindow(this);
    newReorganizationWindow->show();
}

void MainWindow::updateSotcksTable()
{
    // Get assets
    std::vector<std::shared_ptr<Asset>> assets = assetController.getAllAssets();

    // Init row
    int row = 0;

    for(auto asset: assets)
    {
        // Insert new row
        ui->tableWidget_stocks->insertRow(row);

        // Insert data
        ui->tableWidget_stocks->setItem(row, 0, new QTableWidgetItem(asset->getTicker()));
        ui->tableWidget_stocks->setItem(row, 1, new QTableWidgetItem(QString::number(assetController.getAssetDistribution(asset->getTicker()), 'f', 2)));
        ui->tableWidget_stocks->setItem(row, 2, new QTableWidgetItem(QString::number(assetController.getAssetQuantity(asset->getTicker()))));
        ui->tableWidget_stocks->setItem(row, 3, new QTableWidgetItem(QString::number(assetController.getAssetTotalYield(asset->getTicker()), 'f', 2)));
        ui->tableWidget_stocks->setItem(row, 4, new QTableWidgetItem(QString::number(assetController.getAveragePrice(asset->getTicker()), 'f', 2)));
        ui->tableWidget_stocks->setItem(row, 5, new QTableWidgetItem("125.00")); // Preço atual
        ui->tableWidget_stocks->setItem(row, 6, new QTableWidgetItem("4.17%")); // Valorização
        ui->tableWidget_stocks->setItem(row, 7, new QTableWidgetItem("500.00")); // Ganho de capital

        // Increase row
        ++row;
    }
}
