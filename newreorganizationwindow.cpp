#include "newreorganizationwindow.h"
#include "ui_newreorganizationwindow.h"
#include "constants.h"

NewReorganizationWindow::NewReorganizationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewReorganizationWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(384, 215);
    this->setMinimumSize(384, 215);

    // Add reorganization types
    ui->comboBox_reorganizationType->addItem(getReorganizationTypeString(ReorganizationType::DESDOBRAMENTO));
    ui->comboBox_reorganizationType->addItem(getReorganizationTypeString(ReorganizationType::GRUPAMENTO));
    ui->comboBox_reorganizationType->setCurrentIndex(-1);

    // Create ratio validator
    QRegularExpression regexRatio("[0-9]*");
    QRegularExpressionValidator *validatorRatio = new QRegularExpressionValidator(regexRatio, ui->lineEdit_ratio);
    ui->lineEdit_ratio->setValidator(validatorRatio);

    // Set current date
    QDate currentDate = QDate::currentDate();
    ui->dateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->dateEdit->setDate(currentDate);

    // Set labels style
    ui->label_asset->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_data->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_ratio->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_reorganizationType->setStyleSheet("color: rgb(255, 255, 255);");

    // Set buttons style
    ui->pushButton_save->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");
    ui->pushButton_cancel->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");

    // Set combo box style
    ui->comboBox_reorganizationType->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");
    ui->comboBox_asset->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");

    // Set line edit style
    ui->lineEdit_ratio->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");
}

NewReorganizationWindow::~NewReorganizationWindow()
{
    delete ui;
}

void NewReorganizationWindow::on_pushButton_save_clicked()
{

}

void NewReorganizationWindow::on_pushButton_cancel_clicked()
{
    this->close();
}

