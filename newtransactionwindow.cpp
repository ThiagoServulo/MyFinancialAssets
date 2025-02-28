#include "newtransactionwindow.h"
#include "ui_newtransactionwindow.h"
#include "constants.h"
#include "transaction.h"
#include "database.h"
#include "assetapi.h"
#include <QIntValidator>
#include <QMessageBox>

NewTransactionWindow::NewTransactionWindow(InvestmentController *investmentController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewTransactionWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(365, 274);
    this->setMinimumSize(365, 274);

    // Set investiment controller
    this->investmentController = investmentController;

    // Configure labels style
    ui->label_data->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_assetType->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_quantity->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_ticker->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_transactionType->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_value->setStyleSheet("color: rgb(255, 255, 255);");

    // Set buttons style
    ui->pushButton_save->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");
    ui->pushButton_cancel->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);");

    // Set combo box style
    ui->comboBox_assetType->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);"
                                          "border: 1px solid rgb(50, 50, 50);");
    ui->comboBox_transactionType->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255); "
                                                "border: 1px solid rgb(50, 50, 50);");

    // Set line edit style
    ui->lineEdit_quantity->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);"
                                         "border: 1px solid rgb(50, 50, 50);");
    ui->lineEdit_ticker->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);"
                                       "border: 1px solid rgb(50, 50, 50);");
    ui->lineEdit_value->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);"
                                      "border: 1px solid rgb(50, 50, 50);");

    // Set data edit style
    ui->dateEdit->setStyleSheet("background-color: rgb(50, 50, 50); color: rgb(255, 255, 255);"
                                "border: 1px solid rgb(50, 50, 50);");

    // Add assets types
    ui->comboBox_assetType->addItem(getAssetTypeString(AssetType::ACAO));
    ui->comboBox_assetType->addItem(getAssetTypeString(AssetType::FUNDO));
    ui->comboBox_assetType->setCurrentIndex(-1);

    // Add transaction types
    ui->comboBox_transactionType->addItem(getTransactionTypeString(TransactionType::COMPRA));
    ui->comboBox_transactionType->addItem(getTransactionTypeString(TransactionType::VENDA));
    ui->comboBox_transactionType->addItem(getTransactionTypeString(TransactionType::BONIFICACAO));
    ui->comboBox_transactionType->addItem(getTransactionTypeString(TransactionType::RESTITUICAO));
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
        // Create variables
        Database database;
        TransactionType transactionType = getTransactionTypeFromString(ui->comboBox_transactionType->currentText());
        QString ticker = ui->lineEdit_ticker->text();
        int quantity = ui->lineEdit_quantity->text().toInt();
        AssetType assetType = getAssetTypeFromString(ui->comboBox_assetType->currentText());
        QDate date = ui->dateEdit->date();
        double value = ui->lineEdit_value->text().toDouble();

        // Check window type
        if(this->windowTitle() == "Nova Transação")
        {
            if(transactionType == TransactionType::VENDA)
            {
                // Get asset
                auto asset = investmentController->getAsset(ticker);

                // Check asset
                if(asset != nullptr)
                {
                    // Get quantity available
                    int quantityAvailable = investmentController->getAsset(ticker)->getQuantity(nullptr, nullptr);

                    // Check quantity available
                    if(quantityAvailable < quantity)
                    {
                        QMessageBox::critical(this, "Erro", "Você tem : " + QString::number(quantityAvailable) +
                                              " quantidade de papéis disponíveis");
                        return;
                    }
                }
                else
                {
                    QMessageBox::critical(this, "Erro", "Tipo de transação inválida. Você não tem esse papel cadastrado.");
                    return;
                }
            }

            // Add new asset if is necessary
            if(investmentController->getAsset(ticker) == nullptr)
            {
                if(transactionType == TransactionType::COMPRA)
                {
                    // Create asset
                    AssetApi assetApi;
                    Asset asset(ticker, assetType, assetApi.getAssetCurrentPrice(ticker));

                    // Insert asset
                    if(!database.insertAsset(asset))
                    {
                        QMessageBox::critical(this, "Erro", "Erro ao inserir novo ativo");
                        return;
                    }

                    // Add asset to investment controller
                    investmentController->addAsset(std::make_shared<Asset>(asset));
                }
                else
                {
                    QMessageBox::critical(this, "Erro", "Tipo de transação inválida. Você não tem esse papel cadastrado.");
                    return;
                }
            }

            // Create transaction
            Transaction transaction(date, transactionType, quantity, value);

            // Insert transaction
            if(database.insertTransaction(ticker, transaction))
            {
                investmentController->getAsset(ticker)->addEvent(std::make_shared<Transaction>(transaction));
                QMessageBox::information(this, "Sucesso", "Transação inserida com sucesso");
            }
            else
            {
                 QMessageBox::critical(this, "Erro", "Erro ao inserir transação");
            }

            this->close();
        }
        else if(this->windowTitle() == "Editar Transação")
        {
            // TODO: Editar transação
            //this->close();
        }
        else
        {
            QMessageBox::information(this, "Erro", "Tipo de janela inválido");
        }
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

void NewTransactionWindow::on_comboBox_transactionType_textActivated(const QString &arg1)
{
    // Get transaction type
    TransactionType transactionType = getTransactionTypeFromString(arg1);

    if(transactionType == TransactionType::RESTITUICAO || transactionType == TransactionType::BONIFICACAO)
    {
        // Check bonification
        ui->lineEdit_value->setText((transactionType == TransactionType::BONIFICACAO) ? "0": "");
        ui->lineEdit_value->setReadOnly(transactionType == TransactionType::BONIFICACAO);

        // Check bonification
        ui->lineEdit_quantity->setText((transactionType == TransactionType::RESTITUICAO) ? "0": "");
        ui->lineEdit_quantity->setReadOnly(transactionType == TransactionType::RESTITUICAO);
    }
}

void NewTransactionWindow::configureEditWindow(QString ticker, TransactionType transactionType, double price, int quantity, AssetType assetType, QDate date)
{
    ui->lineEdit_ticker->setText(ticker);
    ui->lineEdit_ticker->setReadOnly(true);

    ui->comboBox_assetType->setCurrentIndex(static_cast<int>(assetType));
    ui->comboBox_assetType->setEnabled(false);

    ui->lineEdit_value->setText(QString::number(price));
    ui->lineEdit_quantity->setText(QString::number(quantity));
    ui->comboBox_transactionType->setCurrentIndex(static_cast<int>(transactionType));

    ui->dateEdit->setDisplayFormat("dd/MM/yyyy");
    ui->dateEdit->setDate(date);

    this->setWindowTitle("Editar transação");
}
