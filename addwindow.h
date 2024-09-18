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

private:
    Ui::AddWindow *ui;
};

#endif // ADDWINDOW_H
