#ifndef NEWREORGANIZATIONWINDOW_H
#define NEWREORGANIZATIONWINDOW_H

#include <QMainWindow>
#include "investmentcontroller.h"

namespace Ui {
class NewReorganizationWindow;
}

class NewReorganizationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewReorganizationWindow(InvestmentController *investmentController, QWidget *parent = nullptr);
    ~NewReorganizationWindow();

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::NewReorganizationWindow *ui;
    InvestmentController *investmentController;
};

#endif // NEWREORGANIZATIONWINDOW_H
