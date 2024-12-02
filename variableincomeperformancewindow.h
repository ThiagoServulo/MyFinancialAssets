#ifndef VARIABLEINCOMEPERFORMANCEWINDOW_H
#define VARIABLEINCOMEPERFORMANCEWINDOW_H

#include <QMainWindow>
#include "investmentcontroller.h"

namespace Ui {
class VariableIncomePerformanceWindow;
}

class VariableIncomePerformanceWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const bool MONTH_RESULT = true;
    static const bool YEAR_RESULT = false;
    explicit VariableIncomePerformanceWindow(bool type, InvestmentController *investmentController, QWidget *parent = nullptr);
    ~VariableIncomePerformanceWindow();

private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::VariableIncomePerformanceWindow *ui;
    InvestmentController *investmentController;
    bool windowType;
    void updateMonthTableWidget();
    void updateYearTableWidget();
};

#endif // VARIABLEINCOMEPERFORMANCEWINDOW_H
