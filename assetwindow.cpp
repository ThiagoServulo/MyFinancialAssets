#include "assetwindow.h"
#include "ui_assetwindow.h"
#include "basics.h"
#include "database.h"
#include <QMessageBox>
#include <QMenu>

AssetWindow::AssetWindow(Asset *asset, InvestmentController *investmentController, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AssetWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(1149, 642);
    this->setMinimumSize(1149, 642);

    // Set asset
    this->asset = asset;

    // Set investiment controller
    this->investmentController = investmentController;

    // Update tables
    updateTransactionTable();
    updateYieldTable();

    // Update labels
    ui->label_ticker->setText(asset->getTicker());

    // Configure label styles
    ui->label_ticker->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_transactions->setStyleSheet("color: rgb(255, 255, 255);");
    ui->label_yields->setStyleSheet("color: rgb(255, 255, 255);");

    // Set window background color
    ui->centralwidget->setStyleSheet("background-color: rgb(18, 18, 18);");

    ui->tableWidget_transactions->setContextMenuPolicy(Qt::CustomContextMenu);
}

AssetWindow::~AssetWindow()
{
    delete ui;
}

void AssetWindow::updateTransactionTable()
{
    // Get events
    auto transactions = asset->getTransactions(nullptr, nullptr);
    auto reorganizations = asset->getReorganizations(nullptr, nullptr);
    auto events = mergeAndSortEvents(transactions, reorganizations);

    // Configure transaction table
    QStringList headerLabels = {"Tipo de operação", "Data da operação", "Quantidade", "Preço unitário", "Total da operação",
                                "Quantidade acumulado", "Preço médio", "Valor total investido"};
    configureTableWidget(headerLabels, ui->tableWidget_transactions);

    // Init variables
    int row = 0;
    int accumulatedQuantity = 0;
    double accumulatedTotal = 0;
    double averagePrice = 0;
    QStringList itens;

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

            // Set style
            style = HIGHLIGHT_CELL;

            ReorganizationType reorganizationType = reorganization->getReorganizationType();

            // Check reorganization type
            if(reorganizationType == ReorganizationType::GRUPAMENTO)
            {
                accumulatedQuantity = (accumulatedQuantity == 0) ? 0 : (accumulatedQuantity / reorganization->getRatio());
            }
            else if (reorganizationType == ReorganizationType::DESDOBRAMENTO)
            {
                accumulatedQuantity = static_cast<int>(accumulatedQuantity * reorganization->getRatio());
            }
            else
            {
                throw std::invalid_argument("Reorganization type invalid");
            }

            // Calculate average price and total
            averagePrice = (accumulatedQuantity > 0) ? (accumulatedTotal / accumulatedQuantity) : 0;

            // Populate string list
            itens = {getReorganizationTypeString(reorganizationType), reorganization->getDate().toString("dd/MM/yyyy"),
                     QString::number(reorganization->getRatio()), "-", "-", QString::number(accumulatedQuantity),
                     formatReais(averagePrice), formatReais(accumulatedTotal)};
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
            TransactionType transactionType = transaction->getTransactionType();
            int quantity = transaction->getQuantity();
            double unitaryPrice = transaction->getUnitaryPrice();
            double totalOperation = quantity * unitaryPrice;
            QDate date = transaction->getDate();

            // Check transaction type
            if(transactionType == TransactionType::COMPRA)
            {
                accumulatedQuantity += quantity;
                accumulatedTotal = (accumulatedQuantity > 0) ? accumulatedTotal + totalOperation : 0;
                averagePrice = (accumulatedQuantity > 0) ? (accumulatedTotal / accumulatedQuantity) : 0;
                style = STANDART_CELL;
            }
            else if (transactionType == TransactionType::VENDA)
            {
                accumulatedQuantity -= quantity;
                accumulatedTotal = accumulatedQuantity * averagePrice;
                averagePrice = (accumulatedQuantity > 0) ? averagePrice : 0;
                style = HIGHLIGHT_CELL;
            }
            else if (transactionType == TransactionType::BONIFICACAO)
            {
                accumulatedQuantity += quantity * (quantity / 100.0);
                averagePrice = (accumulatedQuantity > 0) ? (accumulatedTotal / accumulatedQuantity) : 0;
                accumulatedTotal = accumulatedQuantity * averagePrice;
                style = HIGHLIGHT_CELL;
            }
            else if (transactionType == TransactionType::RESTITUICAO)
            {
                accumulatedTotal -= unitaryPrice;
                averagePrice = (accumulatedQuantity > 0) ? (accumulatedTotal / accumulatedQuantity) : 0;
                style = HIGHLIGHT_CELL;
            }
            else
            {
                throw std::invalid_argument("Transaction type invalid");
            }

            // Populate string list
            itens = {getTransactionTypeString(transactionType), date.toString("dd/MM/yyyy"),
                     QString::number(quantity) + ((transactionType == TransactionType::BONIFICACAO) ? "%" : ""),
                     formatReais(unitaryPrice), formatReais(totalOperation),
                     QString::number(accumulatedQuantity), formatReais(averagePrice),
                     formatReais(accumulatedQuantity * averagePrice)};
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

void AssetWindow::updateYieldTable()
{
    // Get Yield
    auto yields = asset->getYields(nullptr, nullptr);

    // Configure yield table
    QStringList headerLabels = {"Tipo de rendimento", "Data do rendimento", "Valor recebido", "Valor agregado"};
    configureTableWidget(headerLabels, ui->tableWidget_yields);

    // Init variables
    int row = 0;
    double accumulatedValue = 0;

    for(auto yield: yields)
    {
        // Get variables
        YieldType yieldType = yield.getYieldType();
        double value = yield.getValue();
        accumulatedValue += value;

        // Set style
        int style = (yieldType == YieldType::DIVIDENDO) ? HIGHLIGHT_CELL : STANDART_CELL;

        // Create string list
        QStringList itens = {getYieldTypeString(yieldType), yield.getDate().toString("dd/MM/yyyy"),
                             formatReais(value), formatReais(accumulatedValue)};

        // Insert itens
        addTableWidgetItens(ui->tableWidget_yields, row, itens, style);

        // Add row
        ++row;
    }
}

void AssetWindow::on_tableWidget_transactions_customContextMenuRequested(const QPoint &pos)
{
    // Get index
    QModelIndex index = ui->tableWidget_transactions->indexAt(pos);

    // Check index
    if (!index.isValid())
    {
        return;
    }

    // Create actions
    QMenu contextMenu;
    QAction *actionDelete = contextMenu.addAction("Excluir");
    QAction *actionEdit = contextMenu.addAction("Editar");

    QAction *selectedAction = contextMenu.exec(ui->tableWidget_transactions->viewport()->mapToGlobal(pos));

    if (selectedAction == actionDelete)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Atenção", "Deseja excluir essa transação?", QMessageBox::Yes | QMessageBox::No);

        if(reply == QMessageBox::Yes)
        {
            // Get current row
            int row = index.row();

            // Get fields
            TransactionType type = getTransactionTypeFromString(ui->tableWidget_transactions->item(row, 0)->text());
            QDate date = QDate::fromString(ui->tableWidget_transactions->item(row, 1)->text(), "dd/MM/yyyy");
            int quantity = ui->tableWidget_transactions->item(row, 2)->text().toInt();
            double price = formatDouble(ui->tableWidget_transactions->item(row, 3)->text());
            QString ticker = ui->label_ticker->text();

            Database database;
            if(database.deleteTransaction(ticker, type, date, quantity, price))
            {
                QMessageBox::information(this, "Sucesso", "Transação excluída com sucesso");

                // Update investiment controller
                Transaction transaction = Transaction(date, type, quantity, price);
                this->investmentController->removeTickerTransaction(ticker, transaction);

                // Update table
                this->updateTransactionTable();
            }
            else
            {
                QMessageBox::information(this, "Erro", "Erro ao excluir transação");
            }
        }
    }
    else if (selectedAction == actionEdit)
    {
        // TODO: Editar uma transação
        QMessageBox::information(this, "Info", "Opção 2 selecionada");
    }
}

