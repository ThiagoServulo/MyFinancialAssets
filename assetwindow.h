#ifndef ASSETWINDOW_H
#define ASSETWINDOW_H

#include "asset.h"
#include "investmentcontroller.h"
#include <QMainWindow>

namespace Ui {
class AssetWindow;
}

class AssetWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssetWindow(Asset *asset, InvestmentController *investmentController, QWidget *parent = nullptr);
    ~AssetWindow();
    void updateTransactionTable();
    void updateYieldTable();

private slots:
    void on_tableWidget_transactions_customContextMenuRequested(const QPoint &pos);
    void on_tableWidget_yields_customContextMenuRequested(const QPoint &pos);

private:
    Ui::AssetWindow *ui;
    Asset *asset;
    InvestmentController *investmentController;
};

#endif // ASSETWINDOW_H
