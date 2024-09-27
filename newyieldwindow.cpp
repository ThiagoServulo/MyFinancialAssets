#include "newyieldwindow.h"
#include "ui_newyieldwindow.h"
#include "database.h"
#include "yield.h"
#include <QMessageBox>

NewYieldWindow::NewYieldWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewYieldWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(362, 214);
    this->setMinimumSize(362, 214);

    // Creating value validator
    QRegularExpression regexValue(R"(\d{0,4}([,.]\d{0,2})?)");
    QRegularExpressionValidator *validatorValue = new QRegularExpressionValidator(regexValue, ui->lineEdit_value);
    ui->lineEdit_value->setValidator(validatorValue);

    // Setting current date
    QDate currentDate = QDate::currentDate();
    ui->dateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->dateEdit->setDate(currentDate);

    Database database;
    if(!database.selectAllTickers(tickers))
    {
        QMessageBox::critical(this, "Erro", "Erro ao ler tickers do banco");
        this->close();
        return;
    }

    // Populate combo box
    ui->comboBox_asset->clear();

    // Iterate through the vector and add each ticker to the combo box
    for (const auto& tickerPair : tickers)
    {
        QString ticker = tickerPair.first;

        // Add ticker to the combo box
        ui->comboBox_asset->addItem(ticker);
    }

    ui->comboBox_asset->setCurrentIndex(-1);
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
        Yield yield(ui->dateEdit->date(), EventType::YIELD, getYieldTypeFromString(ui->comboBox_yieldType->currentText()), ui->lineEdit_value->text().toDouble());

        // Insert yield into database
        Database database;
        int status = database.insertYield(ui->comboBox_asset->currentText(), yield);

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

void NewYieldWindow::on_pushButton_cancel_clicked()
{
    this->close();
}

AssetType NewYieldWindow::findAssetTypeByTicker(const QString& tickerToFind)
{
    for (const auto& tickerPair : tickers)
    {
        if (tickerPair.first == tickerToFind)
        {
            // Return the corresponding AssetType
            return tickerPair.second;
        }
    }

    throw std::invalid_argument("Ticker not found");
}

void NewYieldWindow::on_comboBox_asset_textActivated(const QString &arg1)
{
    ui->comboBox_yieldType->clear();

    AssetType assetType = findAssetTypeByTicker(arg1);

    if(assetType == AssetType::ACAO)
    {
        ui->comboBox_yieldType->addItem(getYieldTypeString(YieldType::DIVIDENDO));
        ui->comboBox_yieldType->addItem(getYieldTypeString(YieldType::JCP));
    }
    else if (assetType == AssetType::FUNDO)
    {
        ui->comboBox_yieldType->addItem(getYieldTypeString(YieldType::RENDIMENTO));
    }
    else
    {
        throw std::invalid_argument("Yield type invalid: " + arg1.toStdString());
    }

    ui->comboBox_yieldType->setCurrentIndex(-1);
}

