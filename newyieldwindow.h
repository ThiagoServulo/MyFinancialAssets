#ifndef NEWYIELDWINDOW_H
#define NEWYIELDWINDOW_H

#include <QMainWindow>
#include "constants.h"
#include "asset.h"
#include "database.h"
#include "assetcontroller.h"

namespace Ui {
class NewYieldWindow;
}

class NewYieldWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewYieldWindow(AssetController *assetController, QWidget *parent = nullptr);
    ~NewYieldWindow();

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();
    void on_comboBox_asset_textActivated(const QString &arg1);

private:
    Ui::NewYieldWindow *ui;
    Database database;
    AssetController *assetController;
};

#endif // NEWYIELDWINDOW_H
