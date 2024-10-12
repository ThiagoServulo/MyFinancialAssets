#ifndef SALESWINDOW_H
#define SALESWINDOW_H

#include <QMainWindow>
#include "assetcontroller.h"

namespace Ui {
class SalesWindow;
}

class SalesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SalesWindow(AssetController *assetController, QWidget *parent = nullptr);
    ~SalesWindow();

private slots:
    void on_tableWidget_sales_cellDoubleClicked(int row, int column);

private:
    Ui::SalesWindow *ui;
    AssetController *assetController;
    void updateTableWidgetSales();
};

#endif // SALESWINDOW_H
