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
#include "editvaluewindow.h"
#include "assetfutureyield.h"
#include <QMessageBox>
#include <QSettings>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1151, 654);
    this->setMinimumSize(1151, 654);

    // Create database
    database.prepareDatabase();

    // Init asset controller
    if(!database.investmentControllerInitialization(&investmentController))
    {
        qDebug() << "Erro in asset controller Initialization";
    }

    // Configure stock and fund tables
    QStringList headerLabels = {"Ticker", "Distribuição", "Quantidade", "Total investido", "Rendimento",
                                "Preço médio", "Preço atual", "Valorização", "Ganho de capital"};
    configureTableWidget(headerLabels, ui->tableWidget_stocks);
    configureTableWidget(headerLabels, ui->tableWidget_funds);

    // Configure fixed income table
    headerLabels = {"Data da compra", "\tDescrição do investimento\t", "Rendimento esperado", " Valor investido ",
                    "   Valor atual   ", "Rendimento", "Data limite"};
    configureTableWidget(headerLabels, ui->tableWidget_fixedIncome);

    // Configure yield calendar table
    headerLabels = {"Ticker", "Data com", "Data do pagamento", "Tipo de rendimento",
                    "Valor pago", "Razão", "Valor recebido"};
    configureTableWidget(headerLabels, ui->tableWidget_yieldCalendar);

    // Init tables
    updateSotckAndFundTable();
    updateFixedIncomeTable();
    updateGeneralTable();
    updateYieldCalendarTable();

    // Set main window background color
    this->setStyleSheet("QMainWindow { background-color: rgb(18, 18, 18); }");

    // Set tab background color
    ui->tab_funds->setStyleSheet("background-color: rgb(18, 18, 18);");
    ui->tab_stocks->setStyleSheet("background-color: rgb(18, 18, 18);");
    ui->tab_general->setStyleSheet("background-color: rgb(18, 18, 18);");
    ui->tab_fixedIncome->setStyleSheet("background-color: rgb(18, 18, 18);");
    ui->tab_yieldCalendar->setStyleSheet("background-color: rgb(18, 18, 18);");

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

    // Change manu bar font color
    menuBar()->setStyleSheet("QMenuBar::item { color: white; }");

    // Change table policy
    ui->tableWidget_stocks->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableWidget_funds->setContextMenuPolicy(Qt::CustomContextMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionTransaction_triggered()
{
    NewTransactionWindow *newTransactionWindow = new NewTransactionWindow(&investmentController, nullptr, this);
    newTransactionWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(newTransactionWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
    newTransactionWindow->show();
}

void MainWindow::on_actionYield_triggered()
{
    NewYieldWindow *newYieldWindow = new NewYieldWindow(&investmentController, nullptr, this);
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
        itens = {asset->getTicker(),
                 formatPercentage(investmentController.getAssetDistribution(asset->getTicker())),
                 QString::number(asset->getQuantity(nullptr, nullptr)),
                 formatReais(asset->getTotalInvested(nullptr, nullptr)),
                 formatReais(asset->getTotalYield(nullptr, nullptr)),
                 formatReais(asset->getAveragePrice()),
                 formatReais(asset->getCurrentPrice()),
                 formatPercentage(asset->getProfitPercentage()),
                 formatReais(asset->getCapitalGain())
                };

        int style = (asset->getQuantity(nullptr, nullptr) == 0) ? HIGHLIGHT_CELL : STANDART_CELL;

        // Check asset type
        if(asset->getAssetType() == AssetType::ACAO)
        {
            // Check checkbox
            if(!ui->checkBox_hideAssets->isChecked() ||
                    (ui->checkBox_hideAssets->isChecked() &&
                     asset->getQuantity(nullptr, nullptr) != 0))
            {
                addTableWidgetItens(ui->tableWidget_stocks, stockRow, itens, style);
                ++stockRow;
            }
        }
        else if(asset->getAssetType() == AssetType::FUNDO)
        {
            // Check checkbox
            if(!ui->checkBox_hideFounds->isChecked() ||
                    (ui->checkBox_hideFounds->isChecked() &&
                     asset->getQuantity(nullptr, nullptr) != 0))
            {
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

    itens = {"Total", "100%", QString::number(totalQuantity), formatReais(totalInvested),
             formatReais(totalYield), "-", "-", "-", formatReais(totalCapitalGain)};

    addTableWidgetItens(ui->tableWidget_funds, fundRow, itens, (HIGHLIGHT_CELL | FONT_BOLD | FONT_SIZE));

    // Add total stock line
    assetType = AssetType::ACAO;
    totalQuantity = investmentController.getTotalQuantityOfAssets(&assetType, nullptr, nullptr);
    totalYield = investmentController.getTotalYieldOfAssets(&assetType, nullptr, nullptr);
    totalInvested = investmentController.getTotalInvestedOfAssets(&assetType, nullptr, nullptr);
    totalCapitalGain = investmentController.getTotalCapitalGainOfAssets(AssetType::ACAO);

    itens = {"Total", "100%", QString::number(totalQuantity), formatReais(totalInvested),
             formatReais(totalYield), "-", "-", "-", formatReais(totalCapitalGain)};

    addTableWidgetItens(ui->tableWidget_stocks, stockRow, itens, (HIGHLIGHT_CELL | FONT_BOLD | FONT_SIZE));
}

void MainWindow::on_tableWidget_stocks_cellDoubleClicked(int row, int column)
{
    // Get ticker
    QString ticker = ui->tableWidget_stocks->item(row , 0)->text();

    // Check ticker
    if(ticker != "Total")
    {
        AssetWindow *assetWindow = new AssetWindow(investmentController.getAsset(ticker).get(),
                                                   &investmentController, this);
        assetWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(assetWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
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
        AssetWindow *assetWindow = new AssetWindow(investmentController.getAsset(ticker).get(),
                                                   &investmentController, this);
        assetWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(assetWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
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
                     formatReais(fixedIncome->getInvestedValue()),
                     formatReais(fixedIncome->getCurrentValue()),
                     formatReais(fixedIncome->getYield()),
                     fixedIncome->getLimitDate().toString("dd/MM/yyyy")
                    };

            // Set status
            int style = (fixedIncome->getLimitDate().month() <= QDate::currentDate().month() &&
                         fixedIncome->getLimitDate().year() <= QDate::currentDate().year()) ?
                         HIGHLIGHT_CELL : STANDART_CELL;

            // Insert itens
            addTableWidgetItens(ui->tableWidget_fixedIncome, row, itens, style);
            ++row;
        }
    }

    // Set itens
    itens = {"-", "Total", "-",
             formatReais(investmentController.getFixedIncomeTotalInvested(FixedIncome::VALID)),
             formatReais(investmentController.getFixedIncomeCurrentTotal(FixedIncome::VALID)),
             formatReais(investmentController.getFixedIncomeTotalYield(FixedIncome::VALID)),
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
                new UpdateFixedIncomeWindow(investmentController.getFixedIncome(purchaseDate, description).get(),
                                            &investmentController, this);
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
    VariableIncomePerformanceWindow *variableIncomeWindow = new
            VariableIncomePerformanceWindow(VariableIncomePerformanceWindow::MONTH_RESULT, &investmentController, this);
    variableIncomeWindow->show();
}

void MainWindow::updateGeneralTable()
{
    // Clear table
    ui->tableWidget_general->clear();
    ui->tableWidget_general->setRowCount(0);
    ui->tableWidget_general->setColumnCount(0);
    ui->tableWidget_general->setHorizontalHeaderLabels({});
    ui->tableWidget_general->setVerticalHeaderLabels({});

    QStringList headers = {"  Mes e ano  "};
    for(auto financialInstitution: investmentController.getFinancialInstitutions())
    {
        // Configure general table
        headers.append(financialInstitution->getName());
    }

    headers.append("Total no mes");

    configureTableWidget(headers, ui->tableWidget_general);

    // Get .ini file
    QString iniFilePath = QCoreApplication::applicationDirPath() + "/config.ini";

    // Create object QSettings to access the .ini
    QSettings settings(iniFilePath, QSettings::IniFormat);

    // Check if the "initDate" key exists
    if (!settings.contains("Date/initDate"))
    {
        // If the key doesn't exist, show an error message box and close the application
        QMessageBox::critical(nullptr, "Error", "'initDate' não foi encontrado no arquivo config.ini.");
        return;
    }

    // Read the initial date
    QString initDateStr = settings.value("Date/initDate", "").toString();

    // Set dates
    QDate *init = new QDate(QDate::fromString("01/" + initDateStr, "dd/MM/yyyy"));
    QDate *end = new QDate(init->addMonths(1));
    QDate *currentDate = new QDate(QDate::currentDate());

    // Init variables
    int row = 0;
    QStringList itens;
    int style = STANDART_CELL;

    QLocale locale(QLocale::Portuguese);

    // Check date
    while ((currentDate->year() > end->year()) ||
           (currentDate->year() == end->year() && currentDate->month() >= end->month() - 1) ||
           (currentDate->year() == end->year() - 1 && currentDate->month() == 12 && end->month() == 1))
    {
        double total = 0;

        itens = {locale.toString(*init, "MMMM") + " " + QString::number(init->year())};

        for(int column = 1; column < ui->tableWidget_general->columnCount(); column++)
        {
            auto financialInstitution = investmentController.getFinancialInstitution(
                        ui->tableWidget_general->horizontalHeaderItem(column)->text());

            if(financialInstitution != NULL)
            {
                auto result = financialInstitution->getFinancialResult(*init);
                itens.append(formatReais((result != nullptr) ? result->getValue() : 0));

                total += ((result != nullptr) ? result->getValue() : 0);
            }
        }

        itens.append(formatReais(total));

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

    ui->tableWidget_general->scrollToBottom();
}

void MainWindow::on_actionInstituition_triggered()
{
    FinancialInstitutionWindow *financialWindow =
            new FinancialInstitutionWindow(nullptr, QDate::currentDate(), &investmentController, this);
    financialWindow->setAttribute(Qt::WA_DeleteOnClose);
    connect(financialWindow, &QObject::destroyed, this, &MainWindow::updateGeneralTable);
    financialWindow->show();
}

void MainWindow::on_tableWidget_general_cellDoubleClicked(int row, int column)
{
    // Get header
    QString header = ui->tableWidget_general->horizontalHeaderItem(column)->text();

    // Get date
    QLocale locale(QLocale::Portuguese);
    QDate date = locale.toDate(ui->tableWidget_general->item(row, 0)->text(), "MMMM yyyy");

    // Check header
    if(!(header.contains("Mes e ano") || header.contains("Total no mes")))
    {
        // Show financial institution window
        FinancialInstitutionWindow *financialWindow =
                new FinancialInstitutionWindow(&header, date, &investmentController, this);
        financialWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(financialWindow, &QObject::destroyed, this, &MainWindow::updateGeneralTable);
        financialWindow->show();
    }
    else if(header.contains("Mes e ano"))
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Atenção",
                                      "Deseja fazer o fechamento de " +
                                      ui->tableWidget_general->item(row, 0)->text() +" ?",
                                      QMessageBox::Yes | QMessageBox::No);


        if(reply == QMessageBox::Yes)
        {
            for(int i = 1; i < ui->tableWidget_general->columnCount() - 1; i++)
            {
                double value = formatDouble(ui->tableWidget_general->item(row, i)->text());
                QString name = ui->tableWidget_general->horizontalHeaderItem(i)->text();
                Database database;

                if(!database.insertFinancialInstitutionMonth(name, FinancialInstitutionMonth(date, value)))
                {
                    QMessageBox::critical(this, "Erro", "Erro ao inserir resultado financeiro da instituição: " + name);
                }
            }

            QMessageBox::information(this, "Fianlizado", "Resultado financeiro inserido");
        }
    }
}

void MainWindow::on_actionAnnualResult_triggered()
{
    VariableIncomePerformanceWindow *variableIncomeWindow = new
            VariableIncomePerformanceWindow(VariableIncomePerformanceWindow::YEAR_RESULT, &investmentController, this);
    variableIncomeWindow->show();
}

void MainWindow::on_tableWidget_stocks_customContextMenuRequested(const QPoint &pos)
{
    processSotckAndFundTableActions(ui->tableWidget_stocks, pos);
}

void MainWindow::updateYieldCalendarTable()
{
    // Init variables
    int row = 0;
    QStringList itens;
    int style = HIGHLIGHT_CELL;
    int lastMonth = -1;
    std::vector<std::tuple<Asset, AssetFutureYield>> yields;

    std::vector<std::shared_ptr<Asset>> assets = investmentController.getAllAssets();

    for (const auto& asset : assets)
    {
        QList<AssetFutureYield> assetYields = getFutureYieldsForTicker(*asset);
        for (const auto& yield : assetYields)
        {
            yields.emplace_back(*asset, yield);
        }
    }

    std::sort(yields.begin(), yields.end(),
        [](const std::tuple<Asset, AssetFutureYield>& a, const std::tuple<Asset, AssetFutureYield>& b)
        {
            return std::get<1>(a).getPaymentDate() < std::get<1>(b).getPaymentDate();
        });

    for (const auto& [asset, yield] : yields)
    {
        int currentMonth = yield.getPaymentDate().month();

        if (currentMonth != lastMonth)
        {
            style = (style == HIGHLIGHT_CELL) ? STANDART_CELL : HIGHLIGHT_CELL;
            lastMonth = currentMonth;
        }

        QDate exYieldDate = yield.getExYieldDate();
        int quantity = asset.getQuantity(nullptr, &exYieldDate);

        if(quantity > 0)
        {
            itens = {asset.getTicker(),
                     exYieldDate.toString("dd/MM/yyyy"),
                     yield.getPaymentDate().toString("dd/MM/yyyy"),
                     getYieldTypeString(yield.getYieldType()),
                     formatReais(yield.getValue()),
                     QString::number(yield.getRatio(), 'f', 2),
                     formatReais(quantity * yield.getValue() * yield.getRatio())
                    };

            addTableWidgetItens(ui->tableWidget_yieldCalendar, row, itens, style);
            ++row;
        }
    }
}

void MainWindow::on_tableWidget_funds_customContextMenuRequested(const QPoint &pos)
{
    processSotckAndFundTableActions(ui->tableWidget_funds, pos);
}

void MainWindow::processSotckAndFundTableActions(QTableWidget *table, const QPoint &pos)
{
    // Get index
    QModelIndex index = table->indexAt(pos);

    // Check index
    if (!index.isValid())
    {
        return;
    }

    // Get current row
    int row = index.row();

    // Create actions
    QMenu contextMenu;
    QAction *actionEditValue = contextMenu.addAction("Editar preço atual");
    QAction *actionNewYield = contextMenu.addAction("Adicionar rendimento");
    QAction *actionNewTransaction = contextMenu.addAction("Adicionar transação");
    QAction *actionDelete = contextMenu.addAction("Excluir ativo");

    QAction *selectedAction = contextMenu.exec(table->viewport()->mapToGlobal(pos));
    QString ticker = table->item(row, 0)->text();

    if (selectedAction == actionEditValue)
    {
        Asset *asset = investmentController.getAsset(ticker).get();
        EditValueWindow *editValueWindow = new EditValueWindow(asset, this);
        editValueWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(editValueWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
        editValueWindow->show();
    }
    else if(selectedAction == actionNewYield)
    {
        Asset *asset = investmentController.getAsset(ticker).get();
        NewYieldWindow *newYieldWindow = new NewYieldWindow(&investmentController, asset, this);
        newYieldWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(newYieldWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
        newYieldWindow->show();
    }
    else if(selectedAction == actionNewTransaction)
    {
        Asset *asset = investmentController.getAsset(ticker).get();
        NewTransactionWindow *newTransactionWindow = new NewTransactionWindow(&investmentController, asset, this);
        newTransactionWindow->setAttribute(Qt::WA_DeleteOnClose);
        connect(newTransactionWindow, &QObject::destroyed, this, &MainWindow::updateSotckAndFundTable);
        newTransactionWindow->show();
    }
    else if(selectedAction == actionDelete)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Atenção",
                                      "Deseja excluir o ativo " + ticker + " ?",
                                      QMessageBox::Yes | QMessageBox::No);

        if(reply == QMessageBox::Yes)
        {
            Database database;
            if(database.deleteAsset(ticker))
            {
                // Show message box
                QMessageBox::information(this, "Sucesso", "Ativo excluído com sucesso");

                // Delete Asset
                investmentController.removeAsset(ticker);

                // Update table
                updateSotckAndFundTable();
            }
            else
            {
                QMessageBox::information(this, "Erro", "Erro ao excluir ativo");
            }
        }
    }
}

