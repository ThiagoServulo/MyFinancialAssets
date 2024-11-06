#include "variableincomeperformancewindow.h"
#include "ui_variableincomeperformancewindow.h"
#include "monthwindow.h"
#include "basics.h"

VariableIncomePerformanceWindow::VariableIncomePerformanceWindow(InvestmentController *investmentController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VariableIncomePerformanceWindow)
{
    ui->setupUi(this);
    //this->setMaximumSize(1061, 600);
    //this->setMinimumSize(1061, 600);

    // Set investment controller
    this->investmentController = investmentController;

    // Set window background color
    ui->centralwidget->setStyleSheet("background-color: rgb(18, 18, 18);");

    // Set label style
    ui->label->setStyleSheet("color: rgb(255, 255, 255);");

    // Configure table widget
    QStringList headerLabels = {"   Mês e ano   ", "Total investido", "Quantidade de ações", "Lucro mensal",
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
    QDate *init = new QDate(2021,1,1);
    QDate *aux = new QDate(2021,1,1);
    QDate *end = new QDate(2021,2,1);
    QDate *currentDate = new QDate(QDate::currentDate());

    // Get assets
    auto assets = investmentController->getAllAssets();

    // Init row
    int row = 0;

    // Check date
    while ((currentDate->year() > end->year()) ||
           (currentDate->year() == end->year() && currentDate->month() >= end->month() - 1))
    {
        // Get itens
        QStringList itens = {aux->toString("MMMM") + " " + QString::number(aux->year()),
                             "R$ " + QString::number(investmentController->getTotalInvestedOfAssets(nullptr, init, end), 'f', 2),
                             QString::number(investmentController->getTotalQuantityOfAssets(nullptr, init, end)),
                             "Lucro mensal",
                             "R$ " + QString::number(investmentController->getTotalYieldOfAssets(nullptr, aux, end), 'f', 2),
                             "R$ " + QString::number(investmentController->getTotalYieldOfAssets(nullptr, init, end), 'f', 2),
                             "Yield acumulado"};

        // Insert total row
        addTableWidgetItens(ui->tableWidget, row, itens, STANDART_CELL);

        // Update variables
        *aux = aux->addMonths(1);
        *end = end->addMonths(1);
        row += 1;
    }
}

void VariableIncomePerformanceWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    QDate date =  QDate::fromString(ui->tableWidget->item(row , 0)->text(), "MMMM yyyy");;

    MonthWindow *monthWindow = new MonthWindow(investmentController, date, this);
    monthWindow->show();
}

