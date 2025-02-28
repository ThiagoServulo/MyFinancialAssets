#include "editvaluewindow.h"
#include "ui_editvaluewindow.h"

EditValueWindow::EditValueWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditValueWindow)
{
    ui->setupUi(this);
}

EditValueWindow::~EditValueWindow()
{
    delete ui;
}
