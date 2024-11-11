#include "mainwindow.h"
#include "newtransactionwindow.h"
#include "newyieldwindow.h"
#include "newreorganizationwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"
#include "asset.h"
#include "event.h"
#include "transaction.h"
#include "yield.h"
#include "assetapi.h"
#include "assetwindow.h"
#include "basics.h"
#include "saleswindow.h"
#include "newfixedincomewindow.h"
#include "updatefixedincomewindow.h"
#include "closedfixedincomeswindow.h"
#include "variableincomeperformancewindow.h"
#include "financialinstitutionwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1151, 654);
    this->setMinimumSize(1151, 654);

    // Create database
    database.prepareDatabase();

    // Configure stock and fund tables
    QStringList headerLabels = {"Ticker", "Distribuição", "Quantidade", "Total investido", "Rendimento",
                                "Preço médio", "Preço atual", "Valorização", "Ganho de capital"};
    configureTableWidget(headerLabels, ui->tableWidget_stocks);
    configureTableWidget(headerLabels, ui->tableWidget_funds);

    // Configure fixed income table
    headerLabels = {"Data da compra", "\tDescrição do investimento\t", "Rendimento esperado", " Valor investido ",
                    "   Valor atual   ", "Rendimento", "Data limite"};
    configureTableWidget(headerLabels, ui->tableWidget_fixedIncome);

    // Init asset controller
    if(!database.investmentControllerInitialization(&investmentController))
    {
        qDebug() << "Erro in asset controller Initialization";
    }

    // Init tables
    updateSotckAndFundTable();
    updateFixedIncomeTable();
    updateGeneralTable();

    // Set main window background color
    this->setStyleSheet("QMainWindow { background-color: rgb(18, 18, 18); }");

    // Set tab background color
    ui->tab_funds->setStyleSheet("background-color: rgb(18, 18, 18);");
    ui->tab_stocks->setStyleSheet("background-color: rgb(18, 18, 18);");
    ui->tab_general->setStyleSheet("background-color: rgb(18, 18, 18);");
    ui->tab_fixedIncome->setStyleSheet("background-color: rgb(18, 18, 18);");

    // Set check boxes style
    ui->checkBox_hideAssets->setStyleSheet("background-color: rgb(18, 18, 18); color: rgb(255, 255, 255);");
    ui->checkBox_hideFounds->setStyleSheet("background-color: rgb(18, 18, 18); color: rgb(255, 255, 255);");

    // Set tab widget style sheet
    ui->tabWidget->setStyleSheet(
        "QTabWidget::pane { background-color: rgb(18, 18, 18); }"
        "QTabBar::tab { background-color: rgb(28, 28, 28); color: rgb(255, 255, 255);"
        "font-size: 14px; font-weight: bold; }"
        "QTabBar::tab:selected { background-color: rgb(18, 18, 18); color: rgb(255, 255, 255);"
        "font-size: 14px; font-weight: bold; }"
    );

    // Set initial tab
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionTransaction_triggered()
{
    NewTransactionWindow *newTransactionWindow = new NewTransactionWindow(&investmentController, this);
    newTransactionWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newTransactionWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
    newTransactionWindow->show();
}

void MainWindow::on_actionYield_triggered()
{
    NewYieldWindow *newYieldWindow = new NewYieldWindow(&investmentController, this);
    newYieldWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newYieldWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
    newYieldWindow->show();
}

void MainWindow::on_actionReorganization_triggered()
{
    NewReorganizationWindow *newReorganizationWindow = new NewReorganizationWindow(&investmentController, this);
    newReorganizationWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newReorganizationWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
    newReorganizationWindow->show();
}

void MainWindow::updateSotckAndFundTable()
{
    // Get assets
    std::vector<std::shared_ptr<Asset>> assets = investmentController.getAllAssets();

    // Clear tables
    ui->tableWidget_stocks->clearContents();
    ui->tableWidget_funds->clearContents();
    ui->tableWidget_stocks->setRowCount(0);
    ui->tableWidget_funds->setRowCount(0);

    // Init variables
    QStringList itens;
    int stockRow = 0;
    int fundRow = 0;

    for(auto asset: assets)
    {
        // Get values
        QString ticker = asset->getTicker();
        int quantity = asset->getQuantity(nullptr, nullptr);
        double totalYield = asset->getTotalYield(nullptr, nullptr);
        double averagePrice = asset->getAveragePrice();
        double currentPrice = asset->getCurrentPrice();
        double totalInvested = asset->getTotalInvested(nullptr, nullptr);
        double captalGain = asset->getCapitalGain();
        double profitPercentage = asset->getProfitPercentage();

        // Check asset type
        if(asset->getAssetType() == AssetType::ACAO)
        {
            // Check checkbox
            if(!ui->checkBox_hideAssets->isChecked() || (ui->checkBox_hideAssets->isChecked() && quantity != 0))
            {
                // Add new line
                itens = {ticker, QString::number(investmentController.getAssetDistribution(ticker), 'f', 2) + "%",
                         QString::number(quantity),
                         "R$ " + QString::number(totalInvested, 'f', 2),
                         "R$ " + QString::number(totalYield, 'f', 2),
                         "R$ " + QString::number(averagePrice, 'f', 2),
                         "R$ " + QString::number(currentPrice, 'f', 2),
                         QString::number(profitPercentage, 'f', 2) + "%",
                         "R$ " + QString::number(captalGain, 'f', 2)};

                int style = (quantity == 0) ? HIGHLIGHT_CELL : STANDART_CELL;
                addTableWidgetItens(ui->tableWidget_stocks, stockRow, itens, style);
                ++stockRow;
            }
        }
        else if(asset->getAssetType() == AssetType::FUNDO)
        {
            // Check checkbox
            if(!ui->checkBox_hideFounds->isChecked() || (ui->checkBox_hideFounds->isChecked() && quantity != 0))
            {
                // Add new line
                itens = {ticker, QString::number(investmentController.getAssetDistribution(ticker), 'f', 2) + "%",
                         QString::number(quantity),
                         "R$ " + QString::number(totalInvested, 'f', 2),
                         "R$ " + QString::number(totalYield, 'f', 2),
                         "R$ " + QString::number(averagePrice, 'f', 2),
                         "R$ " + QString::number(currentPrice, 'f', 2),
                         QString::number(profitPercentage, 'f', 2) + "%",
                         "R$ " + QString::number(captalGain, 'f', 2)};

                int style = (quantity == 0) ? HIGHLIGHT_CELL : STANDART_CELL;
                addTableWidgetItens(ui->tableWidget_funds, fundRow, itens, style);
                ++fundRow;
            }
        }
        else
        {
            throw std::invalid_argument("Asset type invalid");
        }
    }

    // Add total fund line
    AssetType assetType = AssetType::FUNDO;
    int totalQuantity = investmentController.getTotalQuantityOfAssets(&assetType, nullptr, nullptr);
    double totalYield = investmentController.getTotalYieldOfAssets(&assetType, nullptr, nullptr);
    double totalInvested = investmentController.getTotalInvestedOfAssets(&assetType, nullptr, nullptr);
    double totalCapitalGain = investmentController.getTotalCapitalGainOfAssets(AssetType::FUNDO);

    itens = {"Total", "100%", QString::number(totalQuantity),
             "R$ " + QString::number(totalInvested, 'f', 2),
             "R$ " + QString::number(totalYield, 'f', 2), "-", "-", "-",
             "R$ " + QString::number(totalCapitalGain, 'f', 2)};

    addTableWidgetItens(ui->tableWidget_funds, fundRow, itens, (HIGHLIGHT_CELL | FONT_BOLD | FONT_SIZE));

    // Add total stock line
    assetType = AssetType::ACAO;
    totalQuantity = investmentController.getTotalQuantityOfAssets(&assetType, nullptr, nullptr);
    totalYield = investmentController.getTotalYieldOfAssets(&assetType, nullptr, nullptr);
    totalInvested = investmentController.getTotalInvestedOfAssets(&assetType, nullptr, nullptr);
    totalCapitalGain = investmentController.getTotalCapitalGainOfAssets(AssetType::ACAO);

    itens = {"Total", "100%", QString::number(totalQuantity),
             "R$ " + QString::number(totalInvested, 'f', 2),
             "R$ " + QString::number(totalYield, 'f', 2), "-", "-", "-",
             "R$ " + QString::number(totalCapitalGain, 'f', 2)};

    addTableWidgetItens(ui->tableWidget_stocks, stockRow, itens, (HIGHLIGHT_CELL | FONT_BOLD | FONT_SIZE));
}

void MainWindow::on_tableWidget_stocks_cellDoubleClicked(int row, int column)
{
    // Get ticker
    QString ticker = ui->tableWidget_stocks->item(row , 0)->text();

    // Check ticker
    if(ticker != "Total")
    {
        AssetWindow *assetWindow = new AssetWindow(investmentController.getAsset(ticker).get(), this);
        assetWindow->show();
    }
}

void MainWindow::on_tableWidget_funds_cellDoubleClicked(int row, int column)
{
    // Get ticker
    QString ticker = ui->tableWidget_funds->item(row , 0)->text();

    // Check ticker
    if(ticker != "Total")
    {
        AssetWindow *assetWindow = new AssetWindow(investmentController.getAsset(ticker).get(), this);
        assetWindow->show();
    }
}

void MainWindow::on_actionSales_triggered()
{
    SalesWindow *salesWindow = new SalesWindow(&investmentController, this);
    salesWindow->show();
}

void MainWindow::on_checkBox_hideAssets_stateChanged(int arg1)
{
    updateSotckAndFundTable();
}

void MainWindow::on_actionFixedIncome_triggered()
{
    NewFixedIncomeWindow *newFixedIncomeWindow = new NewFixedIncomeWindow(&investmentController, this);
    newFixedIncomeWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newFixedIncomeWindow, &QObject::destroyed, this, &MainWindow::updateFixedIncomeTable);
    newFixedIncomeWindow->show();
}

void MainWindow::updateFixedIncomeTable()
{
    // Set fixed incomes
    auto fixedIncomes = investmentController.getFixedIncomes();

    // Clear table
    ui->tableWidget_fixedIncome->clearContents();
    ui->tableWidget_fixedIncome->setRowCount(0);

    // Set variables
    QStringList itens;
    int row = 0;

    for(auto fixedIncome: fixedIncomes)
    {
        if(fixedIncome->getStatus() == FixedIncome::VALID)
        {
            // Set itens
            itens = {fixedIncome->getPurchaseDate().toString("dd/MM/yyyy"),
                     fixedIncome->getDescription(),
                     fixedIncome->getYieldExpected(),
                     "R$ " + QString::number(fixedIncome->getInvestedValue(), 'f', 2),
                     "R$ " + QString::number(fixedIncome->getCurrentValue(), 'f', 2),
                     "R$ " + QString::number(fixedIncome->getYield(), 'f', 2),
                     fixedIncome->getLimitDate().toString("dd/MM/yyyy")};

            // Set status
            int style = (fixedIncome->getLimitDate() < QDate::currentDate()) ? HIGHLIGHT_CELL : STANDART_CELL;

            // Insert itens
            addTableWidgetItens(ui->tableWidget_fixedIncome, row, itens, style);
            ++row;
        }
    }

    // Set itens
    itens = {"-", "Total", "-",
             "R$ " + QString::number(investmentController.getFixedIncomeTotalInvested(FixedIncome::VALID), 'f', 2),
             "R$ " + QString::number(investmentController.getFixedIncomeCurrentTotal(FixedIncome::VALID), 'f', 2),
             "R$ " + QString::number(investmentController.getFixedIncomeTotalYield(FixedIncome::VALID), 'f', 2),
             QString::number(investmentController.getFixedIncomeTotalYieldPercentage(FixedIncome::VALID), 'f', 2) + "%"};

    // Insert total row
    addTableWidgetItens(ui->tableWidget_fixedIncome, row, itens, (HIGHLIGHT_CELL | FONT_BOLD | FONT_SIZE));
}

void MainWindow::on_tableWidget_fixedIncome_cellDoubleClicked(int row, int column)
{
    // Get fixed income informations
    QString description = ui->tableWidget_fixedIncome->item(row , 1)->text();
    QDate purchaseDate = QDate::fromString(ui->tableWidget_fixedIncome->item(row , 0)->text(), "dd/MM/yyyy");

    // Check description
    if(description != "Total")
    {
        UpdateFixedIncomeWindow *fixedIncomeWindow =
                new UpdateFixedIncomeWindow(investmentController.getFixedIncome(purchaseDate, description).get(), this);
        fixedIncomeWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(fixedIncomeWindow, &QObject::destroyed, this, &MainWindow::updateFixedIncomeTable);
        fixedIncomeWindow->show();
    }
}

void MainWindow::on_checkBox_hideFounds_stateChanged(int arg1)
{
    updateSotckAndFundTable();
}

void MainWindow::on_actionClosedFixedIncomes_triggered()
{
    ClosedFixedIncomesWindow *fixedIncomeWindow = new ClosedFixedIncomesWindow(&investmentController, this);
    fixedIncomeWindow->show();
}

void MainWindow::on_actionVariableIncome_triggered()
{
    VariableIncomePerformanceWindow *variableIncomeWindow = new VariableIncomePerformanceWindow(&investmentController, this);
    variableIncomeWindow->show();
}

void MainWindow::updateGeneralTable()
{
    QStringList headers = {"  Mes e ano  "};
    for(auto financialInstitution: investmentController.getFinancialInstitutions())
    {
        // Configure general table
        headers.append(financialInstitution->getName());
    }

    configureTableWidget(headers, ui->tableWidget_general);

    // Set dates
    QDate *init = new QDate(2021, 1, 1);
    QDate *end = new QDate(2021, 2, 1);
    QDate *currentDate = new QDate(QDate::currentDate());

    // Init variables
    int row = 0;
    QStringList itens ;
    int style = STANDART_CELL;

    // Check date
    while ((currentDate->year() > end->year()) ||
           (currentDate->year() == end->year() && currentDate->month() >= end->month() - 1))
    {
        itens = {init->toString("MMMM") + " " + QString::number(init->year())};
        for(int column = 1; column < ui->tableWidget_general->columnCount(); column++)
        {
            auto financialInstitution = investmentController.getFinancialInstitution(
                        ui->tableWidget_general->horizontalHeaderItem(column)->text());

            auto result = financialInstitution->getFinancialResult(*init);
            itens.append((result != nullptr) ? QString::number(result->getValue(), 'f', 2) : "0");
        }

        // Insert total row
        addTableWidgetItens(ui->tableWidget_general, row, itens, style);

        // Change style
        if(end->year() != init->year())
        {
            style = (style == STANDART_CELL) ? HIGHLIGHT_CELL : STANDART_CELL;
        }

        // Update variables
        *init = init->addMonths(1);
        *end = end->addMonths(1);
        row += 1;
    }
}

void MainWindow::on_actionInstituition_triggered()
{
    FinancialInstitutionWindow *financialWindow =
            new FinancialInstitutionWindow(nullptr, QDate::currentDate(), &investmentController, this);
    financialWindow->show();
}

void MainWindow::on_tableWidget_general_cellDoubleClicked(int row, int column)
{
    // Get header
    QString header = ui->tableWidget_general->horizontalHeaderItem(column)->text();

    // Check header
    if(header.contains("Mes e ano"))
    {
        // Create message box
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Atenção",
                                      "Deseja fazer o encerramento do mes: " +
                                      ui->tableWidget_general->item(row, 0)->text() + "?",
                                      QMessageBox::Yes | QMessageBox::No);

        // Check reply
        if(reply == QMessageBox::Yes)
        {
            for(int i = 1; i < ui->tableWidget_general->columnCount(); i++)
            {
                // Get date
                QDate date = QDate::fromString(ui->tableWidget_general->item(row, 0)->text(), "MMMM yyyy");
                double value = ui->tableWidget_general->item(row, i)->text().toDouble();
                QString name =  ui->tableWidget_general->horizontalHeaderItem(i)->text();
                FinancialInstitutionMonth result(date, value);

                // Insert financial institution month
                Database database;
                database.insertFinancialInstitutionMonth(name, result);
            }
        }
    }
    else
    {
        // Get date
        QDate date = QDate::fromString(ui->tableWidget_general->item(row, 0)->text(), "MMMM yyyy");

        // Show financial institution window
        FinancialInstitutionWindow *financialWindow =
                new FinancialInstitutionWindow(&header, date, &investmentController, this);
        financialWindow->show();
    }
}
