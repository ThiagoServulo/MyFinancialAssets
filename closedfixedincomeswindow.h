#ifndef CLOSEDFIXEDINCOMESWINDOW_H
#define CLOSEDFIXEDINCOMESWINDOW_H

#include <QMainWindow>
#include "investmentcontroller.h"

namespace Ui {
class ClosedFixedIncomesWindow;
}

class ClosedFixedIncomesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClosedFixedIncomesWindow(InvestmentController *investmentController, QWidget *parent = nullptr);
    ~ClosedFixedIncomesWindow();

private:
    Ui::ClosedFixedIncomesWindow *ui;
    InvestmentController *investmentController;
    void updateTableWidget();
};

#endif // CLOSEDFIXEDINCOMESWINDOW_H
