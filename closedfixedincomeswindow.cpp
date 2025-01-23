#include "closedfixedincomeswindow.h"
#include "ui_closedfixedincomeswindow.h"
#include "basics.h"

ClosedFixedIncomesWindow::ClosedFixedIncomesWindow(InvestmentController *investmentController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClosedFixedIncomesWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1061, 600);
    this->setMinimumSize(1061, 600);

    // Set investment controller
    this->investmentController = investmentController;

    // Set window background color
    ui->centralwidget->setStyleSheet("background-color: rgb(18, 18, 18);");

    // Set label style
    ui->label->setStyleSheet("color: rgb(255, 255, 255);");

    // Configure table widget
    QStringList headerLabels = {"Descrição do investimento", "Data da compra", "Data da venda", "  Valor investido  ",
                                "  Valor resgatado  ", "  Rendimento  ", "Rendimento [%]"};
    configureTableWidget(headerLabels, ui->tableWidget);

    // Update table widget
    updateTableWidget();
}

ClosedFixedIncomesWindow::~ClosedFixedIncomesWindow()
{
    delete ui;
}

void ClosedFixedIncomesWindow::updateTableWidget()
{
    // Init variables
    int row = 0;
    QStringList itens;

    auto fixedIncomes = investmentController->getFixedIncomes();

    std::sort(fixedIncomes.begin(), fixedIncomes.end(),
       [](const std::shared_ptr<FixedIncome>& a, const std::shared_ptr<FixedIncome>& b)
    {
        return a->getLimitDate() < b->getLimitDate();
    });

    // Get fixed incomes
    for(auto fixedIncome: fixedIncomes)
    {
        // Check status
        if(fixedIncome->getStatus() == FixedIncome::CLOSED)
        {
            // Set itens
            itens = {fixedIncome->getDescription(), fixedIncome->getPurchaseDate().toString("dd/MM/yyyy"),
                     fixedIncome->getLimitDate().toString("dd/MM/yyyy"),
                     formatReais(fixedIncome->getInvestedValue()), formatReais(fixedIncome->getCurrentValue()),
                     formatReais(fixedIncome->getYield()), QString::number(fixedIncome->getYieldPercentage(), 'f', 2) + "%"};

            // Insert itens
            addTableWidgetItens(ui->tableWidget, row, itens, STANDART_CELL);

            // Update row
            ++row;
        }
    }

    // Create total row
    itens = {"Total", "-", "-",
             formatReais(investmentController->getFixedIncomeTotalInvested(FixedIncome::CLOSED)),
             formatReais(investmentController->getFixedIncomeCurrentTotal(FixedIncome::CLOSED)),
             formatReais(investmentController->getFixedIncomeTotalYield(FixedIncome::CLOSED)),
             QString::number(investmentController->getFixedIncomeTotalYieldPercentage(FixedIncome::CLOSED), 'f', 2) + "%"};

    // Insert total
    addTableWidgetItens(ui->tableWidget, row, itens, (HIGHLIGHT_CELL | FONT_BOLD | FONT_SIZE));
}
