#ifndef ASSETWINDOW_H
#define ASSETWINDOW_H

#include "asset.h"
#include <QMainWindow>

namespace Ui {
class AssetWindow;
}

class AssetWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssetWindow(Asset *asset, QWidget *parent = nullptr);
    ~AssetWindow();
    void updateTransactionTable();
    void updateYieldTable();

private:
    Ui::AssetWindow *ui;
    Asset *asset;
};

#endif // ASSETWINDOW_H
