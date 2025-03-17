#include "editvaluewindow.h"
#include "ui_editvaluewindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>

EditValueWindow::EditValueWindow(Asset *asset, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditValueWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(208, 140);
    this->setMinimumSize(208, 140);

    // Set buttons style
    ui->pushButton_save->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");
    ui->pushButton_cancel->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");

    // Set label style
    ui->label_value->setStyleSheet("color: rgb(255, 255, 255);");

    // Set line edit style
    ui->lineEdit_value->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);"
                                      "border: 1px solid rgb(50, 50, 50);");

    // Set regex to field
    QRegularExpression regexValue(R"(\d{0,8}([.]\d{0,2})?)");
    QRegularExpressionValidator *validatorValue = new QRegularExpressionValidator(regexValue, ui->lineEdit_value);
    ui->lineEdit_value->setValidator(validatorValue);

    this->asset = asset;
}

EditValueWindow::~EditValueWindow()
{
    delete ui;
}

void EditValueWindow::on_pushButton_cancel_clicked()
{
    this->close();
}

void EditValueWindow::on_pushButton_save_clicked()
{
    double value = ui->lineEdit_value->text().toDouble();

    if(value <= 0)
    {
        QMessageBox::information(this, "Erro", "O valor precisa ser maior que zero");
        return;
    }

    this->asset->setCurrentPrice(value);
    this->close();
}

