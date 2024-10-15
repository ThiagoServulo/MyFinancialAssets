#ifndef SALESWINDOW_H
#define SALESWINDOW_H

#include <QMainWindow>
#include "investmentcontroller.h"

namespace Ui {
class SalesWindow;
}

class SalesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SalesWindow(InvestmentController *investmentController, QWidget *parent = nullptr);
    ~SalesWindow();

private slots:
    void on_tableWidget_sales_cellDoubleClicked(int row, int column);

private:
    Ui::SalesWindow *ui;
    InvestmentController *investmentController;
    void updateTableWidgetSales();
};

#endif // SALESWINDOW_H
