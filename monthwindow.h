#ifndef MONTHWINDOW_H
#define MONTHWINDOW_H

#include <QMainWindow>
#include "investmentcontroller.h"

namespace Ui {
class MonthWindow;
}

class MonthWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MonthWindow(InvestmentController *investmentController, QDate date, QWidget *parent = nullptr);
    ~MonthWindow();
    void updateTransactionTable();
    void updateYieldTable();

private:
    Ui::MonthWindow *ui;
    InvestmentController *investmentController;
    QDate initDate;
    QDate endDate;
};

#endif // MONTHWINDOW_H
