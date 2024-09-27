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
        // Create transaction
        Transaction transaction(ui->dateEdit->date(), EventType::TRANSACTION,
                                getTransactionTypeFromString(ui->comboBox_transactionType->currentText()),
                                ui->lineEdit_quantity->text().toInt(),
                                ui->lineEdit_value->text().toDouble());

        // Insert transaction into database
        Database database;
        if(database.insertTransaction(ui->lineEdit_ticker->text(),
                                      getAssetTypeFromString(ui->comboBox_assetType->currentText()), transaction))
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

