#include "database.h"
#include "constants.h"
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
    // Query to create yield table
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS yield_table (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            id_ticker INTEGER NOT NULL,
            id_yield_type INTEGER NOT NULL,
            value DOUBLE NOT NULL,
            date DATE NOT NULL
        );
    )";

    // Execute query
    QSqlQuery query;
    if (!query.exec(createTableQuery))
    {
        qDebug() << "Error to create yield_table";
        return false;
    }

    // Table created
    qDebug() << "Table yield_table created";
    return true;
}

bool Database::createAssetTypeTable()
{
    // Query to create asset type table
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS asset_type_table (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            asset_type TEXT NOT NULL
        );
    )";

    // Execute query
    QSqlQuery query;
    if (!query.exec(createTableQuery))
    {
        qDebug() << "Error to create asset_type_table";
        return false;
    }

    // Table created
    qDebug() << "Table asset_type_table created";
    return true;
}

bool Database::populateAssetTypeTable()
{
    // Query to insert data into asset type table
    QString insertQuery = R"(
        INSERT INTO asset_type_table (asset_type) VALUES
        (:type1),
        (:type2);
    )";

    QSqlQuery query;
    query.prepare(insertQuery);
    query.bindValue(":type1", getAssetTypeString(AssetType::ACAO));
    query.bindValue(":type2", getAssetTypeString(AssetType::FUNDO));

    // Execute query
    if (!query.exec())
    {
        qDebug() << "Error to populate asset_type_table";
        return false;
    }

    // Table populate
    qDebug() << "Table asset_type_table populated";
    return true;
}

bool Database::createYieldTypeTable()
{
    // Query to create yield type table
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS yield_type_table (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            yield_type TEXT NOT NULL
        );
    )";

    // Execute query
    QSqlQuery query;
    if (!query.exec(createTableQuery))
    {
        qDebug() << "Error to create yield_type_table";
        return false;
    }

    // Table created
    qDebug() << "Table yield_type_table created";
    return true;
}

bool Database::populateYieldTypeTable()
{
    // Query to insert data into yield type table
    QString insertQuery = R"(
        INSERT INTO yield_type_table (yield_type) VALUES
        (:type1),
        (:type2),
        (:type3);
    )";

    QSqlQuery query;
    query.prepare(insertQuery);
    query.bindValue(":type1", getYieldTypeString(YieldType::DIVIDENDO));
    query.bindValue(":type2", getYieldTypeString(YieldType::JCP));
    query.bindValue(":type3", getYieldTypeString(YieldType::RENDIMENTO));

    // Execute query
    if (!query.exec())
    {
        qDebug() << "Error to populate yield_type_table";
        return false;
    }

    // Table populate
    qDebug() << "Table yield_type_table populated";
    return true;
}

bool Database::createTransactionTypeTable()
{
    // Query to create transaction type table
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS transaction_type_table (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            transaction_type TEXT NOT NULL
        );
    )";

    // Execute query
    QSqlQuery query;
    if (!query.exec(createTableQuery))
    {
        qDebug() << "Error to create transaction_type_table";
        return false;
    }

    // Table created
    qDebug() << "Table transaction_type_table created";
    return true;
}

bool Database::populateTransactionTypeTable()
{
    // Query to insert data into transaction type table
    QString insertQuery = R"(
        INSERT INTO transaction_type_table (transaction_type) VALUES
        (:type1),
        (:type2);
    )";

    QSqlQuery query;
    query.prepare(insertQuery);
    query.bindValue(":type1", getTransactionTypeString(TransactionType::COMPRA));
    query.bindValue(":type2", getTransactionTypeString(TransactionType::VENDA));

    // Execute query
    if (!query.exec())
    {
        qDebug() << "Error to populate transaction_type_table";
        return false;
    }

    // Table populate
    qDebug() << "Table transaction_type_table populated";
    return true;
}

bool Database::createTickerTable()
{
    // Query to create ticker table
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS ticker_table (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            ticker TEXT NOT NULL,
            id_asset_type INTEGER NOT NULL
        );
    )";

    // Execute query
    QSqlQuery query;
    if (!query.exec(createTableQuery))
    {
        qDebug() << "Error to create ticker_table";
        return false;
    }

    // Table created
    qDebug() << "Table ticker_table created";
    return true;
}

bool Database::prepareDatabase()
{
    bool createStatus = false;

    if(openDatabase())
    {
        createStatus |= createYieldTable();
        createStatus |= createAssetTypeTable();
        createStatus |= createYieldTypeTable();
        createStatus |= createTransactionTypeTable();
        createStatus |= createTickerTable();
        populateAssetTypeTable();
        populateYieldTypeTable();
        populateTransactionTypeTable();
    }
    else
    {
        qDebug() << "Error to open database";
        return createStatus;
    }

    closeDatabase();
    return createStatus;
}
