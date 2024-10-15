#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "investmentcontroller.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionYield_triggered();
    void on_actionTransaction_triggered();
    void on_actionReorganization_triggered();
    void on_tableWidget_stocks_cellDoubleClicked(int row, int column);
    void on_tableWidget_funds_cellDoubleClicked(int row, int column);
    void on_actionSales_triggered();
    void on_checkBox_hideAssets_stateChanged(int arg1);
    void on_actionFixedIncome_triggered();

private:
    Ui::MainWindow *ui;
    InvestmentController investmentController;
    Database database;
    void updateSotckAndFundTable();
    void addNewLineToTable(QTableWidget *tableWidget, int row, QString ticker, QString distribution, QString quantity, QString totalYield, QString averagePrice, QString currentPriceStr, QString profitPercentage, QString capitalGain);
};
#endif // MAINWINDOW_H
