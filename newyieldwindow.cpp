#include "newyieldwindow.h"
#include "ui_newyieldwindow.h"
#include "constants.h"
#include <QMessageBox>

NewYieldWindow::NewYieldWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewYieldWindow)
{
    ui->setupUi(this);

    // Add assets types
    ui->comboBox_assetType->addItem(getAssetTypeString(AssetType::ACAO));
    ui->comboBox_assetType->addItem(getAssetTypeString(AssetType::FUNDO));
    ui->comboBox_assetType->setCurrentIndex(-1);

    // Creating value validator
    QRegularExpression regexValue(R"(\d{0,4}([,.]\d{0,2})?)");
    QRegularExpressionValidator *validatorValue = new QRegularExpressionValidator(regexValue, ui->lineEdit_value);
    ui->lineEdit_value->setValidator(validatorValue);

    // Creating ticker validator
    QRegularExpression regexTicker("[a-zA-Z0-9]*");
    QRegularExpressionValidator *validatorTicker = new QRegularExpressionValidator(regexTicker, ui->lineEdit_ticker);
    ui->lineEdit_ticker->setValidator(validatorTicker);

    // Setting current date
    QDate currentDate = QDate::currentDate();
    ui->dateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->dateEdit->setDate(currentDate);
}

NewYieldWindow::~NewYieldWindow()
{
    delete ui;
}

void NewYieldWindow::on_pushButton_save_clicked()
{
    // Checking fields
    if(isValidOperationType(ui->comboBox_yieldType->currentText()) &&
       isValidAssetType(ui->comboBox_assetType->currentText()) &&
       ui->lineEdit_ticker->text() != "" &&
       ui->lineEdit_value->text() != "")
    {
        QMessageBox::about(this, "Válido", "Válido");
    }
    else
    {
        QMessageBox::about(this, "Inválido", "Inválido");
    }
}

void NewYieldWindow::on_comboBox_assetType_textActivated(const QString &arg1)
{
    ui->comboBox_yieldType->clear();

    if(arg1 == getAssetTypeString(AssetType::ACAO))
    {
        ui->comboBox_yieldType->addItem(getYieldTypeString(YieldType::DIVIDENDO));
        ui->comboBox_yieldType->addItem(getYieldTypeString(YieldType::JCP));
    }
    else // FUNDO
    {
        ui->comboBox_yieldType->addItem(getYieldTypeString(YieldType::RENDIMENTO));
    }

    ui->comboBox_yieldType->setCurrentIndex(-1);
}

void NewYieldWindow::on_pushButton_cancel_clicked()
{
    this->close();
}

