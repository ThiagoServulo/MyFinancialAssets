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
    //Database db;
    //db.prepareDatabase();
    Asset a("teste",AssetType::ACAO);
    std::shared_ptr<Transaction> t = std::make_shared<Transaction>(QDate(2020,5,5), EventType::TRANSACTION, TransactionType::COMPRA, 10, 1.50);
    a.addEvent(t);
    std::vector<Transaction> i = a.getTransactionList();
    for (auto x : i)
    {
        qDebug() << "quantidade: " << x.getQuantity();

    }
}

void MainWindow::on_actionReorganization_triggered()
{
    NewReorganizationWindow *newReorganizationWindow = new NewReorganizationWindow(this);
    newReorganizationWindow->show();
}
