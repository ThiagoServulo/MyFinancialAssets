#include "basics.h"
#include <QtWidgets/QTableWidget>
#include <QHeaderView>

QTableWidgetItem* createStyledItem(QString text, int style)
{
    QTableWidgetItem* item = new QTableWidgetItem(text);

    if(style & HIGHLIGHT_CELL)
    {
       item->setBackground(QColor(38, 38, 38));
    }
    else
    {
        item->setBackground(QColor(28, 28, 28));
    }

    item->setForeground(QColor(255, 255, 255));

    QFont font = item->font();
    item->setFont(font);

    return item;
}


void configureTableWidget(QStringList headerLabels, QTableWidget *tableWidget)
{
    // Configure table
    tableWidget->setColumnCount(headerLabels.size());
    tableWidget->setHorizontalHeaderLabels(headerLabels);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // Set column width
    for(int i = 0; i < headerLabels.size(); i++)
    {
        QFontMetrics fontMetrics(tableWidget->font());
        int width = fontMetrics.horizontalAdvance(headerLabels[i]) + 40;
        tableWidget->setColumnWidth(i, width);
    }

    // Set table widget style
    tableWidget->setStyleSheet(
        "QTableWidget { background-color: rgb(28, 28, 28); color: rgb(255, 255, 255); }"
        "QHeaderView::section { background-color: rgb(44, 44, 46); color: rgb(255, 255, 255); }"
        "QTableView::item:selected { background-color: rgb(65, 65, 65); color: rgb(255, 255, 255); }"
    );

    // Change focus policy
    tableWidget->setFocusPolicy(Qt::NoFocus);

    // Set header style
    QHeaderView* header = tableWidget->horizontalHeader();
    header->setStyleSheet("QHeaderView::section { font-size: 14px; font-weight: bold; background-color: rgb(28, 28, 28); color: rgb(255, 255, 255); }");
}

void addTableWidgetItem(QTableWidget *tableWidget, int row, int column, QString item, int style)
{
    tableWidget->setItem(row, column, createStyledItem(item, style));
}

void addTableWidgetItens(QTableWidget *tableWidget, int row, QStringList itens, int style)
{
    for(int column = 0; column < itens.size(); column++)
    {
        addTableWidgetItem(tableWidget, row, column, itens[column], style);
    }
}
