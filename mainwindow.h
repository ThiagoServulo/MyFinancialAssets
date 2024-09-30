#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "assetcontroller.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionYield_triggered();
    void on_actionTransaction_triggered();
    void on_pushButton_clicked();
    void on_actionReorganization_triggered();

private:
    Ui::MainWindow *ui;
    AssetController assetController;
    Database database;
    void updateSotcksTable();
};
#endif // MAINWINDOW_H
