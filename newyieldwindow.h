#ifndef NEWYIELDWINDOW_H
#define NEWYIELDWINDOW_H

#include <QMainWindow>
#include "constants.h"
#include "asset.h"
#include "database.h"
#include "investmentcontroller.h"

namespace Ui {
class NewYieldWindow;
}

class NewYieldWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NewYieldWindow(InvestmentController *investmentController, QWidget *parent = nullptr);
    explicit NewYieldWindow(InvestmentController *investmentController, Asset *asset, QWidget *parent = nullptr);
    explicit NewYieldWindow(InvestmentController *investmentController, Yield *yield, Asset *asset, QWidget *parent = nullptr);
    ~NewYieldWindow();

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();
    void on_comboBox_asset_textActivated(const QString &arg1);

private:
    Ui::NewYieldWindow *ui;
    Database database;
    InvestmentController *investmentController;
    void initComboBoxYieldType(AssetType assetType);
};

#endif // NEWYIELDWINDOW_H
