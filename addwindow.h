#ifndef ADDWINDOW_H
#define ADDWINDOW_H

#include <QMainWindow>

namespace Ui {
class AddWindow;
}

class AddWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddWindow(QWidget *parent = nullptr);
    ~AddWindow();

private slots:
    void on_pushButton_save_clicked();
    void on_comboBox_type_textActivated(const QString &arg1);

private:
    Ui::AddWindow *ui;
};

#endif // ADDWINDOW_H
