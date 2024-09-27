#include "mainwindow.h"
#include "newtransactionwindow.h"
#include "newyieldwindow.h"
#include "newreorganizationwindow.h"
#include "ui_mainwindow.h"
#include "database.h"

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
    NewYieldWindow *newYieldWindow = new NewYieldWindow(this);
    newYieldWindow->show();
}

void MainWindow::on_pushButton_clicked()
{
    Database db;
    //db.prepareDatabase();

    Yield yield(QDate(2020,5,5), EventType::YIELD, YieldType::DIVIDENDO, 2.55);

    qDebug() << db.insertYield("sapo", yield);
}

void MainWindow::on_actionReorganization_triggered()
{
    NewReorganizationWindow *newReorganizationWindow = new NewReorganizationWindow(this);
    newReorganizationWindow->show();
}
