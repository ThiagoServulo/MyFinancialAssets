#include "newfixedincomewindow.h"
#include "ui_newfixedincomewindow.h"
#include "fixedincome.h"
#include "database.h"
#include <QMessageBox>

NewFixedIncomeWindow::NewFixedIncomeWindow(InvestmentController *investmentController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewFixedIncomeWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(367, 257);
    this->setMinimumSize(367, 257);

    // Set investment controller
    this->investmentController = investmentController;

    // Set labels style
    ui->label_description->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_limitDate->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_puchaseDate->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_valueInvested->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_yield->setStyleSheet("color: rgb(255, 255, 255);");

    // Set buttons style
    ui->pushButton_save->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");
    ui->pushButton_cancel->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");

    // Set data edit style
    ui->dateEdit_limitDate->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");
    ui->dateEdit_puchaseDate->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");

    // Set line edit style
    ui->lineEdit_description->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");
    ui->lineEdit_valueInvested->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");
    ui->lineEdit_yield->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");

    // Set current date
    QDate currentDate = QDate::currentDate();
    ui->dateEdit_puchaseDate->setDisplayFormat("dd/MM/yyyy");
    ui->dateEdit_puchaseDate->setDate(currentDate);
    ui->dateEdit_limitDate->setDisplayFormat("dd/MM/yyyy");
    ui->dateEdit_limitDate->setDate(currentDate);

    // Set line edits limits
    ui->lineEdit_description->setMaxLength(24);
    ui->lineEdit_yield->setMaxLength(10);

    QRegularExpression regexValue(R"(\d{0,8}([.]\d{0,2})?)");
    QRegularExpressionValidator *validatorValue = new QRegularExpressionValidator(regexValue, ui->lineEdit_valueInvested);
    ui->lineEdit_valueInvested->setValidator(validatorValue);
}

NewFixedIncomeWindow::~NewFixedIncomeWindow()
{
    delete ui;
}

void NewFixedIncomeWindow::on_pushButton_save_clicked()
{
    // Set variables
    QString description = ui->lineEdit_description->text();
    QString yieldExpected = ui->lineEdit_yield->text();
    double valueInvested = ui->lineEdit_valueInvested->text().toDouble();
    QDate limitDate = ui->dateEdit_limitDate->date();
    QDate puchaseDate = ui->dateEdit_puchaseDate->date();

    // Check fields
    if(description != "" && yieldExpected != "" && valueInvested > 0 && limitDate > puchaseDate)
    {
        FixedIncome fixedIncome(puchaseDate, description, yieldExpected, valueInvested, limitDate, 0, FixedIncome::VALID);

        // Insert fixed income into database
        Database database;
        if(database.insertFixedIncome(fixedIncome))
        {
            investmentController->addFixedIncome(std::make_shared<FixedIncome>(fixedIncome));
            QMessageBox::information(this, "Sucesso", "Renda fixa inserida com sucesso");
        }
        else
        {
            QMessageBox::critical(this, "Erro", "Erro ao inserir renda fixa");
        }

        // Close window
        this->close();
    }
    else
    {
        QMessageBox::information(this, "Inválido", "Um dos campos digitados está inválido");
    }
}

void NewFixedIncomeWindow::on_pushButton_cancel_clicked()
{
    this->close();
}

