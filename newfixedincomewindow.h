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

private:
    Ui::NewFixedIncomeWindow *ui;
};

#endif // NEWFIXEDINCOMEWINDOW_H
