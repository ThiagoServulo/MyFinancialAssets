#include "monthwindow.h"
#include "ui_monthwindow.h"
#include "basics.h"
#include "constants.h"

MonthWindow::MonthWindow(InvestmentController *investmentController, QDate date, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonthWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(991, 843);
    this->setMinimumSize(991, 843);

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
    ui->label_fixedIncome->setStyleSheet("color: rgb(255, 255, 255);");

    // Set window background color
    ui->centralwidget->setStyleSheet("background-color: rgb(18, 18, 18);");

    // Configure transaction table
    QStringList headerLabels = {"Ticker", "Tipo de operação", "Data da operação", "Quantidade",
                                "Preço unitário", "Total da operação"};
    configureTableWidget(headerLabels, ui->tableWidget_transactions);

    // Configure yield table
    headerLabels = {"Ticker", "Tipo de rendimento", "Data do rendimento", "Valor recebido"};
    configureTableWidget(headerLabels, ui->tableWidget_yields);

    // Configure yield table
    headerLabels = {"  Status  ", "Descrição do investimento", "Data da compra", "Data da venda",
                    " Valor investido ", "  Valor recebido  ", "   Rendimento   "};
    configureTableWidget(headerLabels, ui->tableWidget_fixedIncome);

    // Update tables
    updateTransactionTable();
    updateYieldTable();
    updateFixedIncomeTable();
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

    // Create new variable
    std::vector<std::tuple<QString, Yield>> consolidatedYields;

     // Process assets
     for (auto asset : assets)
     {
         // Get asset's yields
         auto yields = asset->getYields(&initDate, &endDate);

         for (auto yield : yields)
         {
             // Add yield and ticker
             consolidatedYields.emplace_back(asset->getTicker(), yield);
         }
     }

     // Sort yields by date
     std::sort(consolidatedYields.begin(), consolidatedYields.end(),
       [](const std::tuple<QString, Yield>& a, const std::tuple<QString, Yield>& b)
       {
           return std::get<1>(a).getEventDate() < std::get<1>(b).getEventDate();
       });

     // Insert yields to the table
     int row = 0;
     for (const auto& [ticker, yield] : consolidatedYields)
     {
         // Get values
         YieldType yieldType = yield.getYieldType();
         double value = yield.getValue();

         // Set the style
         int style = (yieldType == YieldType::RENDIMENTO) ? HIGHLIGHT_CELL : STANDART_CELL;

         // Create itens
         QStringList itens = {ticker, getYieldTypeString(yieldType),
                              yield.getEventDate().toString("dd/MM/yyyy"), formatReais(value)};

         // Insert new row
         addTableWidgetItens(ui->tableWidget_yields, row, itens, style);

         // Increment row
         ++row;
     }
}

void MonthWindow::updateFixedIncomeTable()
{
    // Get date
    QDate auxDate = endDate.addDays(-1);

    // Init variables
    int row = 0;

    // Add purchased fixed incomes
    for(auto fixedIncome: investmentController->getPurchasedFixedIncomes(&auxDate))
    {
        // Get variables
        QString description = fixedIncome->getDescription();
        QDate purchaseDate = fixedIncome->getPurchaseDate();
        QDate limitDate = fixedIncome->getLimitDate();
        double investedValue = fixedIncome->getInvestedValue();

        // Create string list
        QStringList itens = {"Comprado", description, purchaseDate.toString("dd/MM/yyyy"),
                             limitDate.toString("dd/MM/yyyy"), formatReais(investedValue),
                             "-", "-", "-"};

        // Insert itens
        addTableWidgetItens(ui->tableWidget_fixedIncome, row, itens, STANDART_CELL);

        // Add row
        ++row;
    }

    // Add sold fixed incomes
    for(auto fixedIncome: investmentController->getSoldFixedIncomes(&auxDate))
    {
        // Get variables
        QString description = fixedIncome->getDescription();
        QDate purchaseDate = fixedIncome->getPurchaseDate();
        QDate limitDate = fixedIncome->getLimitDate();
        double investedValue = fixedIncome->getInvestedValue();
        double currentValue = fixedIncome->getCurrentValue();
        double yield = fixedIncome->getYield();
        double yieldPercentage = fixedIncome->getYieldPercentage();

        // Create string list
        QStringList itens = {"Encerrado", description, purchaseDate.toString("dd/MM/yyyy"),
                             limitDate.toString("dd/MM/yyyy"), formatReais(investedValue),
                             formatReais(currentValue), formatReais(yield) + " [" +
                             QString::number(yieldPercentage, 'f', 2) + "%]"};

        // Insert itens
        addTableWidgetItens(ui->tableWidget_fixedIncome, row, itens, HIGHLIGHT_CELL);

        // Add row
        ++row;
    }
}
