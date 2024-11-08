#include "financialinstitutionwindow.h"
#include "ui_financialinstitutionwindow.h"
#include "financialinstitution.h"
#include "database.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>

FinancialInstitutionWindow::FinancialInstitutionWindow(QString *name, QDate date, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FinancialInstitutionWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(367, 159);
    this->setMinimumSize(367, 159);

    // Update variables
    this->name = name;
    this->date = date;

    // Update label
    if(this->name != nullptr)
    {
        ui->label_institutionName->setText(*name);
    }

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

void FinancialInstitutionWindow::on_pushButton_save_clicked()
{
    // Check fields
    if(ui->lineEdit_institutionName->text() != "" && ui->lineEdit_value->text() != "")
    {
        Database database;
        if(this->name == nullptr)
        {
            FinancialInstitution financialInstitution(ui->lineEdit_institutionName->text(), this->date,
                                                      ui->lineEdit_value->text().toDouble());

            qDebug() << "aaaaa";
            /*if(database.insertFinancialInstitution(financialInstitution))
            {
                //this->investmentController->getAsset(ui->comboBox_asset->currentText())->addEvent(
                //                                     std::make_shared<Reorganization>(reorganization));
                QMessageBox::information(this, "Sucesso", "Instituição financeira inserida com sucesso");
            }
            else
            {
                QMessageBox::critical(this, "Erro", "Erro ao inserir instituição financeira");
            }*/
        }
        else
        {
            // Atualizar o valor
        }
    }
}

void FinancialInstitutionWindow::on_pushButton_cancel_clicked()
{
    this->close();
}

