#ifndef FINANCIALINSTITUTIONWINDOW_H
#define FINANCIALINSTITUTIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class FinancialInstitutionWindow;
}

class FinancialInstitutionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FinancialInstitutionWindow(QWidget *parent = nullptr);
    ~FinancialInstitutionWindow();

private:
    Ui::FinancialInstitutionWindow *ui;
};

#endif // FINANCIALINSTITUTIONWINDOW_H
