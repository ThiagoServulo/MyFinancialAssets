#include "financialinstitutionwindow.h"
#include "ui_financialinstitutionwindow.h"
#include <QRegularExpressionValidator>

FinancialInstitutionWindow::FinancialInstitutionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FinancialInstitutionWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(367, 159);
    this->setMinimumSize(367, 159);

    // Update label
    //ui->label_ticker->setText(asset->getTicker());

    // Configure label styles
    ui->label_institutionName->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_value->setStyleSheet("color: rgb(255, 255, 255);");

    // Set line edit style
    ui->lineEdit_institutionName->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);"
                                            "border: 1px solid rgb(50, 50, 50);");
    ui->lineEdit_value->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);"
                                              "border: 1px solid rgb(50, 50, 50);");

    // Set regex to field
    QRegularExpression regexValue(R"(\d{0,8}([.]\d{0,2})?)");
    QRegularExpressionValidator *validatorValue = new QRegularExpressionValidator(regexValue, ui->lineEdit_value);
    ui->lineEdit_value->setValidator(validatorValue);

    // Set buttons style
    ui->pushButton_save->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");
    ui->pushButton_cancel->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");

    // Set window background color
    ui->centralwidget->setStyleSheet("background-color: rgb(18, 18, 18);");
}

FinancialInstitutionWindow::~FinancialInstitutionWindow()
{
    delete ui;
}
