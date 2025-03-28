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
    void on_tableWidget_fixedIncome_cellDoubleClicked(int row, int column);
    void on_checkBox_hideFounds_stateChanged(int arg1);
    void on_actionClosedFixedIncomes_triggered();
    void on_actionVariableIncome_triggered();
    void on_actionInstituition_triggered();
    void on_tableWidget_general_cellDoubleClicked(int row, int column);
    void on_actionAnnualResult_triggered();
    void on_tableWidget_stocks_customContextMenuRequested(const QPoint &pos);
    void on_tableWidget_funds_customContextMenuRequested(const QPoint &pos);
    void on_tableWidget_yieldCalendar_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    InvestmentController investmentController;
    Database database;
    void updateSotckAndFundTable();
    void updateFixedIncomeTable();
    void updateGeneralTable();
    void updateYieldCalendarTable();
    void processSotckAndFundTableActions(QTableWidget *table, const QPoint &pos);

};
#endif // MAINWINDOW_H
