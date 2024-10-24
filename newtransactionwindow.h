#ifndef NEWTRANSACTIONWINDOW_H
#define NEWTRANSACTIONWINDOW_H

#include <QMainWindow>
#include "investmentcontroller.h"

namespace Ui {
class NewTransactionWindow;
}

class NewTransactionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewTransactionWindow(InvestmentController *investmentController, QWidget *parent = nullptr);
    ~NewTransactionWindow();

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::NewTransactionWindow *ui;
    InvestmentController *investmentController;
};

#endif // NEWTRANSACTIONWINDOW_H
