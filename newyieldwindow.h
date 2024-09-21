#ifndef NEWYIELDWINDOW_H
#define NEWYIELDWINDOW_H

#include <QMainWindow>

namespace Ui {
class NewYieldWindow;
}

class NewYieldWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewYieldWindow(QWidget *parent = nullptr);
    ~NewYieldWindow();

private slots:
    void on_pushButton_save_clicked();
    void on_comboBox_assetType_textActivated(const QString &arg1);
    void on_pushButton_cancel_clicked();

private:
    Ui::NewYieldWindow *ui;
};

#endif // NEWYIELDWINDOW_H
