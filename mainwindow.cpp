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
    //qDebug() << db.insertTicker("SAPO", AssetType::ACAO);

    //Database db;
    //db.prepareDatabase();
    /*
    Asset a("teste",AssetType::ACAO);
    std::shared_ptr<Transaction> t = std::make_shared<Transaction>(QDate(2020,5,5), EventType::TRANSACTION, TransactionType::COMPRA, 10, 1.50);
    std::shared_ptr<Transaction> t2 = std::make_shared<Transaction>(QDate(2020,5,5), EventType::TRANSACTION, TransactionType::COMPRA, 18, 1.50);
    std::shared_ptr<Yield> t3 = std::make_shared<Yield>(QDate(2020,5,5), EventType::YIELD, YieldType::DIVIDENDO, 20);
    a.addEvent(t);
    a.addEvent(t2);
    a.addEvent(t3);
    std::vector<Transaction> i = a.getTransactionList();
    for (auto x : i)
    {
        qDebug() << "quantidade: " << x.getQuantity();
    }
    std::vector<Yield> i2 = a.getYieldList();
    for (auto x : i2)
    {
        qDebug() << "valor: " << x.getValue();

    }
    */
}

void MainWindow::on_actionReorganization_triggered()
{
    NewReorganizationWindow *newReorganizationWindow = new NewReorganizationWindow(this);
    newReorganizationWindow->show();
}
