#ifndef EDITVALUEWINDOW_H
#define EDITVALUEWINDOW_H

#include <QMainWindow>
#include "asset.h"

namespace Ui {
class EditValueWindow;
}

class EditValueWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditValueWindow(Asset *asset, QWidget *parent = nullptr);
    ~EditValueWindow();

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_save_clicked();

private:
    Ui::EditValueWindow *ui;
    Asset * asset;
};

#endif // EDITVALUEWINDOW_H
