#ifndef BASICS_H
#define BASICS_H

#include <QTableWidgetItem>
#include <QComboBox>
#include "investmentcontroller.h"
#include "reorganization.h"

#define STANDART_CELL   0
#define HIGHLIGHT_CELL (1 << 0) // Bit 0
#define FONT_BOLD      (1 << 1) // Bit 1
#define FONT_SIZE      (1 << 2) // Bit 2

QTableWidgetItem* createStyledItem(QString text, int style);
void configureTableWidget(QStringList headerLabels, QTableWidget *tableWidget);
void addTableWidgetItem(QTableWidget *tableWidget, int row, int column, QString item, int style);
void addTableWidgetItens(QTableWidget *tableWidget, int row, QStringList itens, int style);
void initComboBoxAssets(QComboBox *comboBox, InvestmentController *investmentcontroller);
std::vector<Event*> mergeAndSortEvents(const std::vector<Transaction>& transactions, const std::vector<Reorganization>& reorganizations);
QDate getEarliestTransactionDate(std::vector<Transaction>& transactions, TransactionType transactionType);

#endif // BASICS_H
