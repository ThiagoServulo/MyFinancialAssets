#include "basics.h"
#include <QtWidgets/QTableWidget>
#include <QHeaderView>

QTableWidgetItem* createStyledItem(QString text, int style)
{
    QTableWidgetItem* item = new QTableWidgetItem(text);

    // Set background color
    item->setBackground((style & HIGHLIGHT_CELL) ? QColor(38, 38, 38) : QColor(28, 28, 28));

    // Set text color
    item->setForeground(QColor(255, 255, 255));

    // Set text alignment
    item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    // Create font
    QFont font = item->font();

    // Set bold
    font.setBold(style & FONT_BOLD);

    // Set point size
    font.setPointSize((style & FONT_SIZE) ? 12 : 10);

    // Use font
    item->setFont(font);

    // Return item
    return item;
}

void configureTableWidget(QStringList headerLabels, QTableWidget *tableWidget)
{
    // Configure table
    tableWidget->clear();
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
        int width = fontMetrics.horizontalAdvance(headerLabels[i]) + 50;
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
    // Add item
    tableWidget->setItem(row, column, createStyledItem(item, style));
}

void addTableWidgetItens(QTableWidget *tableWidget, int row, QStringList itens, int style)
{
    // Insert row
    tableWidget->insertRow(row);

    // Add item
    for(int column = 0; column < itens.size(); column++)
    {
        addTableWidgetItem(tableWidget, row, column, itens[column], style);
    }
}

void initComboBoxAssets(QComboBox *comboBox, AssetController *assetController)
{
    // Clear combo box
    comboBox->clear();

    // Iterate to add each ticker to the combo box
    for (auto asset : assetController->getAllAssets())
    {
        // Add ticker to the combo box
        comboBox->addItem(asset->getTicker());
    }

    // Set current index
    comboBox->setCurrentIndex(-1);
}

std::vector<Event*> mergeAndSortEvents(const std::vector<Transaction>& transactions, const std::vector<Reorganization>& reorganizations)
{
    std::vector<Event*> events;

    // Add transactions
    for (const auto& transaction : transactions)
    {
        events.push_back(const_cast<Transaction*>(&transaction));
    }

    // Add reorganizations
    for (const auto& reorganization : reorganizations)
    {
        events.push_back(const_cast<Reorganization*>(&reorganization));
    }

    // Sort events by date
    std::sort(events.begin(), events.end(), [](Event* a, Event* b)
    {
        return a->getDate() < b->getDate();
    });

    return events;
}
