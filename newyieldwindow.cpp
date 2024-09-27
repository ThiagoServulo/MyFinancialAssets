#include "newyieldwindow.h"
#include "ui_newyieldwindow.h"
#include "constants.h"
#include "database.h"
#include "yield.h"
#include <QMessageBox>

NewYieldWindow::NewYieldWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewYieldWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(367, 268);
    this->setMinimumSize(367, 268);

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
    if(isValidYieldType(ui->comboBox_yieldType->currentText()) &&
       isValidAssetType(ui->comboBox_assetType->currentText()) &&
       ui->lineEdit_ticker->text() != "" &&
       ui->lineEdit_value->text() != "")
    {
        // Create yield
        Yield yield(ui->dateEdit->date(), EventType::YIELD, getYieldTypeFromString(ui->comboBox_yieldType->currentText()), ui->lineEdit_value->text().toDouble());

        // Insert yield into database
        Database database;
        int status = database.insertYield(ui->lineEdit_ticker->text(), yield);

        switch (status)
        {
            case DATABASE_SUCCESS:
                QMessageBox::information(this, "Sucesso", "Rendimento inserido com sucesso");
            break;

            case NOT_FOUND:
                QMessageBox::information(this, "Erro", "Esse ativo não está cadastrado no banco");
            break;

            default:
                QMessageBox::critical(this, "Erro", "Erro ao inserir rendimento");

        }

        this->close();
    }
    else
    {
        QMessageBox::information(this, "Inválido", "Um dos campos digitados está inválido");
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
    else if (arg1 == getAssetTypeString(AssetType::FUNDO))
    {
        ui->comboBox_yieldType->addItem(getYieldTypeString(YieldType::RENDIMENTO));
    }
    else
    {
        throw std::invalid_argument("Yield type invalid: " + arg1.toStdString());
    }

    ui->comboBox_yieldType->setCurrentIndex(-1);
}

void NewYieldWindow::on_pushButton_cancel_clicked()
{
    this->close();
}

