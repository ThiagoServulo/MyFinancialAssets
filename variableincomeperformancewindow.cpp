#include "variableincomeperformancewindow.h"
#include "ui_variableincomeperformancewindow.h"
#include "monthwindow.h"
#include "basics.h"

VariableIncomePerformanceWindow::VariableIncomePerformanceWindow(bool type, InvestmentController *investmentController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VariableIncomePerformanceWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1062, 600);
    this->setMinimumSize(1062, 600);

    // Set investment controller
    this->investmentController = investmentController;

    // Set window type
    windowType = type;

    // Set window background color
    ui->centralwidget->setStyleSheet("background-color: rgb(18, 18, 18);");

    // Set label style
    ui->label->setStyleSheet("color: rgb(255, 255, 255);");

    if(windowType == MONTH_RESULT)
    {
        // Set window text
        ui->label->setText("Renda variável: análise mensal");

        // Configure table widget
        QStringList headerLabels = {"     Mês e ano     ", "Total investido", "Quantidade de ações", "Investimento mensal",
                                    "Dividendos no mês", "Dividendos totais", "Yield acumulado"};
        configureTableWidget(headerLabels, ui->tableWidget);

        // Update table widget
        updateMonthTableWidget();
    }
    else
    {
        // Set window text
        ui->label->setText("Renda variável: análise anual");

        // Configure table widget
        QStringList headerLabels = {"  Ano  ", "Quantidade de ações", "Investimento total", "Investimento acumulado",
                                    "Dividendo total", "Dividendo médio", "Dividendo acumulado"};
        configureTableWidget(headerLabels, ui->tableWidget);

        // Update table widget
        updateYearTableWidget();
    }
}

VariableIncomePerformanceWindow::~VariableIncomePerformanceWindow()
{
    delete ui;
}

void VariableIncomePerformanceWindow::updateMonthTableWidget()
{
    // Set dates
    QDate *init = new QDate(2021, 1, 1);
    QDate *aux = new QDate(2021, 1, 1);
    QDate *end = new QDate(2021, 2, 1);
    QDate *currentDate = new QDate(QDate::currentDate());

    // Get assets
    auto assets = investmentController->getAllAssets();

    // Init variables
    int row = 0;
    QStringList itens ;
    int style = STANDART_CELL;
    QLocale locale(QLocale::Portuguese);

    // Check date
    while ((currentDate->year() > end->year()) ||
           (currentDate->year() == end->year() && currentDate->month() >= end->month() - 1) ||
           (currentDate->year() == end->year() - 1 && currentDate->month() == 12 && end->month() == 1))
    {
        // Get itens
        itens = {locale.toString(*aux, "MMMM") + " " + QString::number(aux->year()),
                 formatReais(investmentController->getTotalInvestedOfAssets(nullptr, init, end)),
                 QString::number(investmentController->getTotalQuantityOfAssets(nullptr, init, end)),
                 formatReais(investmentController->getTotalInvestedOfAssets(nullptr, aux, end)),
                 formatReais(investmentController->getTotalYieldOfAssets(nullptr, aux, end)),
                 formatReais(investmentController->getTotalYieldOfAssets(nullptr, init, end)),
                 QString::number((investmentController->getTotalYieldOfAssets(nullptr, init, end) * 100) /
                                  investmentController->getTotalInvestedOfAssets(nullptr, init, end), 'f', 2) + "%"};

        // Insert total row
        addTableWidgetItens(ui->tableWidget, row, itens, style);

        // Change style
        if(end->year() != aux->year())
        {
            style = (style == STANDART_CELL) ? HIGHLIGHT_CELL : STANDART_CELL;
        }

        // Update variables
        *aux = aux->addMonths(1);
        *end = end->addMonths(1);
        row += 1;
    }

    ui->tableWidget->scrollToBottom();
}

void VariableIncomePerformanceWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    // Check window type
    if(windowType == MONTH_RESULT)
    {
        // Set date
        QLocale locale(QLocale::Portuguese);
        QDate date = locale.toDate(ui->tableWidget->item(row, 0)->text(), "MMMM yyyy");

        // Show month window
        MonthWindow *monthWindow = new MonthWindow(investmentController, date, this);
        monthWindow->show();
    }
}

void VariableIncomePerformanceWindow::updateYearTableWidget()
{
    // Set dates
    QDate *init = new QDate(2021, 1, 1);
    QDate *aux = new QDate(2021, 1, 1);
    QDate *end = new QDate(2022, 1, 1);
    QDate *currentDate = new QDate(QDate::currentDate());

    // Get assets
    auto assets = investmentController->getAllAssets();

    // Init variables
    int row = 0;
    QStringList itens ;
    int style = STANDART_CELL;

    // Check date
    while (currentDate->year() >= end->year() - 1)
    {
        // Get month divisor for avarage yield
        int month = (currentDate->year() == end->year() - 1) ? currentDate->month() : 12;

        // Get itens
        itens = {QString::number(aux->year()),
                 QString::number(investmentController->getTotalQuantityOfAssets(nullptr, init, end)),
                 formatReais(investmentController->getTotalInvestedOfAssets(nullptr, aux, end)),
                 formatReais(investmentController->getTotalInvestedOfAssets(nullptr, init, end)),
                 formatReais(investmentController->getTotalYieldOfAssets(nullptr, aux, end)),
                 formatReais(investmentController->getTotalYieldOfAssets(nullptr, aux, end) / month),
                 formatReais(investmentController->getTotalYieldOfAssets(nullptr, init, end))};

        // Insert total row
        addTableWidgetItens(ui->tableWidget, row, itens, style);

        // Change style
        if(end->year() != aux->year())
        {
            style = (style == STANDART_CELL) ? HIGHLIGHT_CELL : STANDART_CELL;
        }

        // Update variables
        *aux = aux->addYears(1);
        *end = end->addYears(1);
        row += 1;
    }

    ui->tableWidget->scrollToBottom();
}
