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
    explicit VariableIncomePerformanceWindow(InvestmentController *investmentController, QWidget *parent = nullptr);
    ~VariableIncomePerformanceWindow();

private:
    Ui::VariableIncomePerformanceWindow *ui;
    InvestmentController *investmentController;
    void updateTableWidget();
};

#endif // VARIABLEINCOMEPERFORMANCEWINDOW_H
