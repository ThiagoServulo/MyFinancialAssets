#include "variableincomeperformancewindow.h"
#include "ui_variableincomeperformancewindow.h"
#include "monthwindow.h"
#include "basics.h"

VariableIncomePerformanceWindow::VariableIncomePerformanceWindow(InvestmentController *investmentController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VariableIncomePerformanceWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1062, 600);
    this->setMinimumSize(1062, 600);

    // Set investment controller
    this->investmentController = investmentController;

    // Set window background color
    ui->centralwidget->setStyleSheet("background-color: rgb(18, 18, 18);");

    // Set label style
    ui->label->setStyleSheet("color: rgb(255, 255, 255);");

    // Configure table widget
    QStringList headerLabels = {"     Mês e ano     ", "Total investido", "Quantidade de ações", "Investimento mensal",
                                "Dividendos no mês", "Dividendos totais", "Yield acumulado"};
    configureTableWidget(headerLabels, ui->tableWidget);

    // Update table widget
    updateTableWidget();
}

VariableIncomePerformanceWindow::~VariableIncomePerformanceWindow()
{
    delete ui;
}

void VariableIncomePerformanceWindow::updateTableWidget()
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
           (currentDate->year() == end->year() && currentDate->month() >= end->month() - 1))
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
    // Set date
    QLocale locale(QLocale::Portuguese);
    QDate date = locale.toDate(ui->tableWidget->item(row, 0)->text(), "MMMM yyyy");

    // Show month window
    MonthWindow *monthWindow = new MonthWindow(investmentController, date, this);
    monthWindow->show();
}

