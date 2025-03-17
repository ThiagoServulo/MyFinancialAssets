#include "newyieldwindow.h"
#include "ui_newyieldwindow.h"
#include "yield.h"
#include <QMessageBox>
#include "basics.h"

NewYieldWindow::NewYieldWindow(InvestmentController *investmentController, Asset *asset, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewYieldWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(362, 214);
    this->setMinimumSize(362, 214);

    // Set investiment controller
    this->investmentController = investmentController;

    // Set labels style
    ui->label_asset->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_data->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_value->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_yieldType->setStyleSheet("color: rgb(255, 255, 255);");

    // Set buttons style
    ui->pushButton_save->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");
    ui->pushButton_cancel->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");

    // Set combo box style
    ui->comboBox_yieldType->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");
    ui->comboBox_asset->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");

    // Set data edit style
    ui->dateEdit->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");

    // Set line edit style
    ui->lineEdit_value->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); border: 1px solid rgb(50, 50, 50);");

    // Create value validator
    QRegularExpression regexValue(R"(\d{0,4}([.]\d{0,2})?)");
    QRegularExpressionValidator *validatorValue = new QRegularExpressionValidator(regexValue, ui->lineEdit_value);
    ui->lineEdit_value->setValidator(validatorValue);

    // Set current date
    QDate currentDate = QDate::currentDate();
    ui->dateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->dateEdit->setDate(currentDate);

    // Init combo box assets
    initComboBoxAssets(ui->comboBox_asset, investmentController);

    if(asset != nullptr)
    {
        ui->comboBox_asset->setCurrentText(asset->getTicker());
        ui->comboBox_asset->setEnabled(false);
        initComboBoxYieldType(asset->getAssetType());
    }
}

NewYieldWindow::~NewYieldWindow()
{
    delete ui;
}

void NewYieldWindow::on_pushButton_save_clicked()
{
    // Checking fields
    if(isValidYieldType(ui->comboBox_yieldType->currentText()) &&
       ui->comboBox_asset->currentText() != "" &&
       ui->lineEdit_value->text() != "")
    {
        // Create yield
        Yield yield(ui->dateEdit->date(), getYieldTypeFromString(ui->comboBox_yieldType->currentText()), ui->lineEdit_value->text().toDouble());

        // Insert yield into database
        Database database;
        if(database.insertYield(ui->comboBox_asset->currentText(), yield))
        {
            this->investmentController->getAsset(ui->comboBox_asset->currentText())->addEvent(std::make_shared<Yield>(yield));
            QMessageBox::information(this, "Sucesso", "Rendimento inserido com sucesso");
        }
        else
        {
            QMessageBox::critical(this, "Erro", "Erro ao inserir rendimento");
        }

        // Close window
        this->close();
    }
    else
    {
        QMessageBox::information(this, "Inválido", "Um dos campos digitados está inválido");
    }
}

void NewYieldWindow::on_pushButton_cancel_clicked()
{
    this->close();
}

void NewYieldWindow::on_comboBox_asset_textActivated(const QString &arg1)
{
    AssetType assetType = investmentController->getAsset(arg1)->getAssetType();

    initComboBoxYieldType(assetType);
}

void NewYieldWindow::initComboBoxYieldType(AssetType assetType)
{
    // Clear all previous items from the combo box
    ui->comboBox_yieldType->clear();

    // Check the asset type
    if (assetType == AssetType::ACAO)
    {
        // Add options for stocks
        ui->comboBox_yieldType->addItem(getYieldTypeString(YieldType::DIVIDENDO));
        ui->comboBox_yieldType->addItem(getYieldTypeString(YieldType::JCP));
        ui->comboBox_yieldType->addItem(getYieldTypeString(YieldType::REST_CAPITAL));
    }
    else if (assetType == AssetType::FUNDO)
    {
        // Add option for funds
        ui->comboBox_yieldType->addItem(getYieldTypeString(YieldType::RENDIMENTO));
    }
    else
    {
        // Throw an exception if the asset type is invalid
        throw std::invalid_argument("Yield type invalid");
    }

    // Set the combo box to an unselected state by default
    ui->comboBox_yieldType->setCurrentIndex(-1);
}

