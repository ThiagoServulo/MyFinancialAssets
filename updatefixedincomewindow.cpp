#include "updatefixedincomewindow.h"
#include "ui_updatefixedincomewindow.h"
#include "database.h"
#include <QMessageBox>

UpdateFixedIncomeWindow::UpdateFixedIncomeWindow(FixedIncome *fixedIncome, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UpdateFixedIncomeWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(364, 301);
    this->setMinimumSize(364, 301);

    // Set fixed income
    this->fixedIncome = fixedIncome;

    // Set labels style
    ui->label_description->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_limitDate->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_puchaseDate->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_valueInvested->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_currentValue->setStyleSheet("color: rgb(255, 255, 255);");

    // Set buttons style
    ui->pushButton_conclude->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");
    ui->pushButton_cancel->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");
    ui->pushButton_remove->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");
    ui->pushButton_update->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");

    // Set data edit style
    ui->dateEdit_limitDate->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");
    ui->dateEdit_puchaseDate->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");

    // Set line edit style
    ui->lineEdit_description->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");
    ui->lineEdit_valueInvested->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");
    ui->lineEdit_currentValue->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");

    // Set read only fields
    ui->lineEdit_description->setReadOnly(true);
    ui->lineEdit_valueInvested->setReadOnly(true);
    ui->dateEdit_limitDate->setReadOnly(true);
    ui->dateEdit_puchaseDate->setReadOnly(true);

    // Set fields values
    ui->lineEdit_description->setText(this->fixedIncome->getDescription());
    ui->lineEdit_valueInvested->setText(QString::number(fixedIncome->getInvestedValue(), 'f', 2));
    ui->lineEdit_currentValue->setText(QString::number(fixedIncome->getCurrentValue(), 'f', 2));
    ui->dateEdit_puchaseDate->setDate(fixedIncome->getPurchaseDate());
    ui->dateEdit_limitDate->setDate(fixedIncome->getLimitDate());

    // Set regex to field
    QRegularExpression regexValue(R"(\d{0,8}([.]\d{0,2})?)");
    QRegularExpressionValidator *validatorValue = new QRegularExpressionValidator(regexValue, ui->lineEdit_valueInvested);
    ui->lineEdit_currentValue->setValidator(validatorValue);
}

UpdateFixedIncomeWindow::~UpdateFixedIncomeWindow()
{
    delete ui;
}

void UpdateFixedIncomeWindow::on_pushButton_update_clicked()
{
    // Get current value
    double currentValue = ui->lineEdit_currentValue->text().toDouble();

    // Check current value
    if(currentValue < 0)
    {
        QMessageBox::information(this, "Inválido", "Insira um valor atual válido");
    }
    else
    {
        Database database;

        // TODO: Fazer a rotina de atualizar o fixed income no database;

        fixedIncome->setCurrentValue(currentValue);
        QMessageBox::information(this, "Sucesso", "Valor atual atualizado com sucesso");
        this->close();
    }
}

void UpdateFixedIncomeWindow::on_pushButton_conclude_clicked()
{
    // TODO: aproveitar a função de atualizar pra mudar o status do fixed income
}

void UpdateFixedIncomeWindow::on_pushButton_remove_clicked()
{

}

void UpdateFixedIncomeWindow::on_pushButton_cancel_clicked()
{
    this->close();
}
