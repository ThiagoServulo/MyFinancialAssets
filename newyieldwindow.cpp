#include "newyieldwindow.h"
#include "ui_newyieldwindow.h"

NewYieldWindow::NewYieldWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewYieldWindow)
{
    ui->setupUi(this);
}

NewYieldWindow::~NewYieldWindow()
{
    delete ui;
}
