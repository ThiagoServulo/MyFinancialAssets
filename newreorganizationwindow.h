#ifndef NEWREORGANIZATIONWINDOW_H
#define NEWREORGANIZATIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class NewReorganizationWindow;
}

class NewReorganizationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewReorganizationWindow(QWidget *parent = nullptr);
    ~NewReorganizationWindow();

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::NewReorganizationWindow *ui;
};

#endif // NEWREORGANIZATIONWINDOW_H
