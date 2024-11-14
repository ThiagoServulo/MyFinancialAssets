#include "monthwindow.h"
#include "ui_monthwindow.h"
#include "basics.h"
#include "constants.h"

MonthWindow::MonthWindow(InvestmentController *investmentController, QDate date, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonthWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(811, 641);
    this->setMinimumSize(811, 641);

    // Set variables
    this->investmentController = investmentController;
    this->initDate = date;
    this->endDate = date.addMonths(1);

    // Set label month text
    QLocale locale(QLocale::Portuguese);
    ui->label_month->setText(locale.toString(date, "MMMM") + " " + QString::number(date.year()));

    // Configure label styles
    ui->label_month->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_transactions->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_yields->setStyleSheet("color: rgb(255, 255, 255);");

    // Set window background color
    ui->centralwidget->setStyleSheet("background-color: rgb(18, 18, 18);");

    // Configure transaction table
    QStringList headerLabels = {"Ticker", "Tipo de operação", "Data da operação", "Quantidade",
                                "Preço unitário", "Total da operação"};
    configureTableWidget(headerLabels, ui->tableWidget_transactions);

    // Configure yield table
    headerLabels = {"Ticker", "Tipo de rendimento", "Data do rendimento", "Valor recebido"};
    configureTableWidget(headerLabels, ui->tableWidget_yields);

    // Update tables
    updateTransactionTable();
    updateYieldTable();
}

MonthWindow::~MonthWindow()
{
    delete ui;
}

void MonthWindow::updateTransactionTable()
{
    // Get assets
    auto assets = investmentController->getAllAssets();

    // Init variables
    int row = 0;
    QStringList itens;

    // Process assets
    for(auto asset: assets)
    {
        // Get events
        auto transactions = asset->getTransactions(&initDate, &endDate);
        auto reorganizations = asset->getReorganizations(&initDate, &endDate);
        auto events = mergeAndSortEvents(transactions, reorganizations);

        // Process events
        for(auto event: events)
        {
            int style;

            if(event->getEventType() == EventType::REORGANIZATION)
            {
                // Cast to reorganization
                Reorganization *reorganization = dynamic_cast<Reorganization*>(event);

                // Check cast
                if (!reorganization)
                {
                    qDebug() << "Reorganization cast error";
                    continue;
                }

                // Get variables
                ReorganizationType reorganizationType = reorganization->getReorganizationType();
                QString ticker = asset->getTicker();
                QDate date = reorganization->getDate();
                int ratio = reorganization->getRatio();
                style = HIGHLIGHT_CELL;

                // Populate string list
                itens = {ticker, getReorganizationTypeString(reorganizationType),
                         date.toString("dd/MM/yyyy"),
                         QString::number(ratio), "-", "-" };
            }
            else if(event->getEventType() == EventType::TRANSACTION)
            {
                // Cast to transaction
                Transaction *transaction = dynamic_cast<Transaction*>(event);

                // Check cast
                if (!transaction)
                {
                    qDebug() << "Transaction cast error";
                    continue;
                }

                // Get variables
                QString ticker = asset->getTicker();
                TransactionType transactionType = transaction->getTransactionType();
                int quantity = transaction->getQuantity();
                double unitaryPrice = transaction->getUnitaryPrice();
                double totalOperation = quantity * unitaryPrice;
                QDate date = transaction->getDate();
                style = STANDART_CELL;

                // Populate string list
                itens = {ticker, getTransactionTypeString(transactionType), date.toString("dd/MM/yyyy"),
                         QString::number(quantity) + ((transactionType == TransactionType::BONIFICACAO) ? "%" : ""),
                        formatReais(unitaryPrice), formatReais(totalOperation)};
            }
            else
            {
                qDebug() << "Invalid event type";
                continue;
            }

            // Insert itens
            addTableWidgetItens(ui->tableWidget_transactions, row, itens, style);

            // Add row
            ++row;
        }
    }
}

void MonthWindow::updateYieldTable()
{
    // Get assets
    auto assets = investmentController->getAllAssets();

    // Init variables
    int row = 0;

    // Process assets
    for(auto asset: assets)
    {
        // Get Yield
        auto yields = asset->getYields(&initDate, &endDate);

        for(auto yield: yields)
        {
            // Get variables
            YieldType yieldType = yield.getYieldType();
            double value = yield.getValue();
            QString ticker = asset->getTicker();

            // Set style
            int style = (yieldType == YieldType::DIVIDENDO) ? HIGHLIGHT_CELL : STANDART_CELL;

            // Create string list
            QStringList itens = {ticker, getYieldTypeString(yieldType),
                                 yield.getDate().toString("dd/MM/yyyy"), formatReais(value)};

            // Insert itens
            addTableWidgetItens(ui->tableWidget_yields, row, itens, style);

            // Add row
            ++row;
        }
    }
}
