#ifndef NEWFIXEDINCOMEWINDOW_H
#define NEWFIXEDINCOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class NewFixedIncomeWindow;
}

class NewFixedIncomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewFixedIncomeWindow(QWidget *parent = nullptr);
    ~NewFixedIncomeWindow();

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::NewFixedIncomeWindow *ui;
};

#endif // NEWFIXEDINCOMEWINDOW_H
