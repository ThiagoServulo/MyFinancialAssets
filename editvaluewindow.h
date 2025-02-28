#ifndef EDITVALUEWINDOW_H
#define EDITVALUEWINDOW_H

#include <QMainWindow>

namespace Ui {
class EditValueWindow;
}

class EditValueWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditValueWindow(QWidget *parent = nullptr);
    ~EditValueWindow();

private:
    Ui::EditValueWindow *ui;
};

#endif // EDITVALUEWINDOW_H
