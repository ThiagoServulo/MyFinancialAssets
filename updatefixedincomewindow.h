#ifndef UPDATEFIXEDINCOMEWINDOW_H
#define UPDATEFIXEDINCOMEWINDOW_H

#include <QMainWindow>
#include "fixedincome.h"
#include "investmentcontroller.h"

namespace Ui {
class UpdateFixedIncomeWindow;
}

class UpdateFixedIncomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UpdateFixedIncomeWindow(FixedIncome *fixedIncome,
                                     InvestmentController *investmentController,
                                     QWidget *parent = nullptr);
    ~UpdateFixedIncomeWindow();

private slots:
    void on_pushButton_update_clicked();
    void on_pushButton_conclude_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::UpdateFixedIncomeWindow *ui;
    FixedIncome *fixedIncome;
    InvestmentController *investmentController;
};

#endif // UPDATEFIXEDINCOMEWINDOW_H
