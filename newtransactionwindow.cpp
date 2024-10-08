#include "newtransactionwindow.h"
#include "ui_newtransactionwindow.h"
#include "constants.h"
#include "transaction.h"
#include "database.h"
#include <QIntValidator>
#include <QMessageBox>

NewTransactionWindow::NewTransactionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewTransactionWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(365, 274);
    this->setMinimumSize(365, 274);

    // Configure labels style
    ui->label_data->setStyleSheet("color: rgb(255, 255, 255);");

    // Add assets types
    ui->comboBox_assetType->addItem(getAssetTypeString(AssetType::ACAO));
    ui->comboBox_assetType->addItem(getAssetTypeString(AssetType::FUNDO));
    ui->comboBox_assetType->setCurrentIndex(-1);

    // Add transaction types
    ui->comboBox_transactionType->addItem(getTransactionTypeString(TransactionType::COMPRA));
    ui->comboBox_transactionType->addItem(getTransactionTypeString(TransactionType::VENDA));
    ui->comboBox_transactionType->setCurrentIndex(-1);

    // Creating quantity validator
    QIntValidator *validatorQuantity = new QIntValidator(0, 9999, ui->lineEdit_quantity);
    ui->lineEdit_quantity->setValidator(validatorQuantity);

    // Creating value validator
    QRegularExpression regexValue(R"(\d{0,4}([.]\d{0,2})?)");
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

NewTransactionWindow::~NewTransactionWindow()
{
    delete ui;
}

void NewTransactionWindow::on_pushButton_save_clicked()
{
    // Checking fields
    if(isValidTransactionType(ui->comboBox_transactionType->currentText()) &&
       isValidAssetType(ui->comboBox_assetType->currentText()) &&
       ui->lineEdit_quantity->text() != "" &&
       ui->lineEdit_ticker->text() != "" &&
       ui->lineEdit_value->text() != "")
    {
        // Check transaction type
        TransactionType transactionType = getTransactionTypeFromString(ui->comboBox_transactionType->currentText());

        Database database;
        QString ticker = ui->lineEdit_ticker->text();
        int quantity = ui->lineEdit_quantity->text().toInt();

        if(transactionType == TransactionType::VENDA)
        {
            int quantityAvailable = database.getTickerQuantity(ticker);

            switch (quantityAvailable)
            {
                case NOT_FOUND:
                    QMessageBox::critical(this, "Erro", "Você não tem esse ativo cadastrado, portanto não pode vendê-lo");
                    return;

                case DATABASE_ERROR:
                    QMessageBox::critical(this, "Erro", "Erro ao inserir transação");
                    this->close();
                    return;

                default:
                    if(quantityAvailable < quantity)
                    {
                        QMessageBox::critical(this, "Erro", "Você tem apenas: " + QString::number(quantityAvailable) + " quantidade de papéis disponíveis");
                        return;
                    }
            }
        }

        // Create transaction
        Transaction transaction(ui->dateEdit->date(), transactionType, quantity, ui->lineEdit_value->text().toDouble());

        // Insert transaction into database
        if(database.insertTransaction(ticker, getAssetTypeFromString(ui->comboBox_assetType->currentText()), transaction))
        {
            QMessageBox::information(this, "Sucesso", "Transação inserida com sucesso");
        }
        else
        {
            QMessageBox::critical(this, "Erro", "Erro ao inserir transação");
        }

        this->close();
    }
    else
    {
        QMessageBox::information(this, "Inválido", "Um dos campos digitados está inválido");
    }
}

void NewTransactionWindow::on_pushButton_cancel_clicked()
{
    this->close();
}

