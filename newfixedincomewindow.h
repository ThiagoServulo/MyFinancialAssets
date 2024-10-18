#ifndef NEWFIXEDINCOMEWINDOW_H
#define NEWFIXEDINCOMEWINDOW_H

#include <QMainWindow>
#include "investmentcontroller.h"

namespace Ui {
class NewFixedIncomeWindow;
}

class NewFixedIncomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewFixedIncomeWindow(InvestmentController *investmentController, QWidget *parent = nullptr);
    ~NewFixedIncomeWindow();

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::NewFixedIncomeWindow *ui;
    InvestmentController *investmentController;
};

#endif // NEWFIXEDINCOMEWINDOW_H
