#ifndef ASSETWINDOW_H
#define ASSETWINDOW_H

#include <QMainWindow>

namespace Ui {
class AssetWindow;
}

class AssetWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AssetWindow(QString ticker, QWidget *parent = nullptr);
    ~AssetWindow();
    void updateTransactionTable(QString ticker);
    void updateYieldTable(QString ticker);

private:
    Ui::AssetWindow *ui;
};

#endif // ASSETWINDOW_H
