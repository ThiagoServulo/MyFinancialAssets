#include "variableincomeperformancewindow.h"
#include "ui_variableincomeperformancewindow.h"
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
                                "Dividendos totais", "Dividendos no mês", "Yield acumulado"};
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
    QDate *init = new QDate(2021,1,1);
    QDate *end = new QDate(2021,2,1);

    auto assets = investmentController->getAllAssets();
    int aux = 0;
    for(auto asset: assets)
    {
        aux += investmentController->getAsset(asset->getTicker())->getQuantity(init, end);
        qDebug() << investmentController->getAsset(asset->getTicker())->getTotalInvested(init, end);
        qDebug() << investmentController->getAsset(asset->getTicker())->getTotalYield(init, end);
    }
    //init->addMonths()
    qDebug() << "aa: " << aux;
}
