#include "database.h"
#include <QApplication>
#include <QSqlQuery>

Database::Database()
{
    // Set SQLite
    database = QSqlDatabase::addDatabase("QSQLITE");

    // Get database name
    QString path = qApp->applicationDirPath() + "/database.db";

    // Set database name
    this->database.setDatabaseName(path);
}

bool Database::openDatabase()
{
    // Open database
    return this->database.open();
}

void Database::closeDatabase()
{
    // Close database
    this->database.close();
}

bool Database::createYieldTable()
{
    // Open database
    if(openDatabase())
    {
        // Query to create yield table
        QString createTableQuery = R"(
            CREATE TABLE IF NOT EXISTS yield_table (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                id_asset_type INTEGER NOT NULL,
                id_ticker INTEGER NOT NULL,
                id_yield_type INTEGER NOT NULL,
                value DOUBLE NOT NULL,
                date DATE NOT NULL
            );
        )";

        QSqlQuery query;
        if (!query.exec(createTableQuery))
        {
            qDebug() << "Error to create yield_table";
            closeDatabase();
            return false;
        }

        // Close database
        closeDatabase();
        return true;
    }

    qDebug() << "Error to open database";
    return false;
}

bool Database::prepareDatabase()
{
    return createYieldTable();
}
