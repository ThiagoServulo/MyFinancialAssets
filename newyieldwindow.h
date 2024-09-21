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

private:
    Ui::NewYieldWindow *ui;
};

#endif // NEWYIELDWINDOW_H
