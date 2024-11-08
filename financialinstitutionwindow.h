#ifndef FINANCIALINSTITUTIONWINDOW_H
#define FINANCIALINSTITUTIONWINDOW_H

#include <QMainWindow>
#include <QDate>

namespace Ui {
class FinancialInstitutionWindow;
}

class FinancialInstitutionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FinancialInstitutionWindow(QString *name, QDate date, QWidget *parent = nullptr);
    ~FinancialInstitutionWindow();

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::FinancialInstitutionWindow *ui;
    QString *name;
    QDate date;
};

#endif // FINANCIALINSTITUTIONWINDOW_H
