#include "database.h"
#include "constants.h"
#include <QApplication>
#include <QSqlQuery>
#include <QFile>
#include <QFileInfo>

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

    // Populate table
    return populateAssetTypeTable();;
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

    // Populate table
    return populateYieldTypeTable();
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

    // Populate table
    return populateTransactionTypeTable();
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

bool Database::createTransactionTable()
{
    // Query to create transaction table
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS transaction_table (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            id_ticker INTEGER NOT NULL,
            id_transaction INTEGER NOT NULL,
            quantity INTEGER NOT NULL,
            unitary_price DOUBLE NOT NULL,
            date DATE NOT NULL
        );
    )";

    // Execute query
    QSqlQuery query;
    if (!query.exec(createTableQuery))
    {
        qDebug() << "Error to create transaction_table";
        return false;
    }

    // Table created
    qDebug() << "Table transaction_table created";
    return true;
}

bool Database::createReorganizationTypeTable()
{
    // Query to create reorganization type table
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS reorganization_type_table (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            reorganization_type TEXT NOT NULL
        );
    )";

    // Execute query
    QSqlQuery query;
    if (!query.exec(createTableQuery))
    {
        qDebug() << "Error to create reorganization_type_table";
        return false;
    }

    // Table created
    qDebug() << "Table reorganization_type_table created";

    // Populate table
    return populateReorganizationTypeTable();
}

bool Database::populateReorganizationTypeTable()
{
    // Query to insert data into reorganization type table
    QString insertQuery = R"(
        INSERT INTO reorganization_type_table (reorganization_type) VALUES
        (:type1),
        (:type2);
    )";

    QSqlQuery query;
    query.prepare(insertQuery);
    query.bindValue(":type1", getReorganizationTypeString(ReorganizationType::DESDOBRAMENTO));
    query.bindValue(":type2", getReorganizationTypeString(ReorganizationType::GRUPAMENTO));

    // Execute query
    if (!query.exec())
    {
        qDebug() << "Error to populate reorganization_type_table";
        return false;
    }

    // Table populate
    qDebug() << "Table reorganization_type_table populated";
    return true;
}

bool Database::createReorganizationTable()
{
    // Query to create reorganization table
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS reorganization_table (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            id_ticker INTEGER NOT NULL,
            id_reorganization INTEGER NOT NULL,
            ratio INTEGER NOT NULL,
            date DATE NOT NULL
        );
    )";

    // Execute query
    QSqlQuery query;
    if (!query.exec(createTableQuery))
    {
        qDebug() << "Error to create reorganization_table";
        return false;
    }

    // Table created
    qDebug() << "Table reorganization_table created";
    return true;
}

bool Database::checkIfDatabaseExists()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString dbPath = appDir + "/database.db";
    QFile file(dbPath);

    return file.exists();
}

bool Database::prepareDatabase()
{
    bool createStatus = false;

    if(checkIfDatabaseExists())
    {
        return true;
    }

    // Creating database
    if(openDatabase())
    {
        qDebug() << "Creating database";
        createStatus |= createYieldTable();
        createStatus |= createAssetTypeTable();
        createStatus |= createYieldTypeTable();
        createStatus |= createTransactionTypeTable();
        createStatus |= createTickerTable();
        createStatus |= createTransactionTable();
        createStatus |= createReorganizationTypeTable();
        createStatus |= createReorganizationTable();
    }
    else
    {
        qDebug() << "Error to create database";
        return createStatus;
    }

    closeDatabase();
    return createStatus;
}

int Database::getAssetTypeId(AssetType assetType)
{
    if(openDatabase())
    {
        QString queryStr = R"(
            SELECT id FROM asset_type_table WHERE asset_type = :type;
        )";

        QSqlQuery query;
        query.prepare(queryStr);
        query.bindValue(":type", getAssetTypeString(assetType));

        if (!query.exec())
        {
            qDebug() << "Error fetching asset type id";
            closeDatabase();
            return DATABASE_ERROR;
        }

        if (query.next())
        {
            // Return the id
            closeDatabase();
            return query.value(0).toInt();
        }

        qDebug() << "Asset type not found";
        closeDatabase();
        return NOT_FOUND;
    }

    qDebug() << "Error to open database to get asset type id";
    return DATABASE_ERROR;
}

int Database::getTickerId(QString ticker)
{
    if(openDatabase())
    {
        QString queryStr = R"(
            SELECT id FROM ticker_table WHERE ticker = :ticker;
        )";

        QSqlQuery query;
        query.prepare(queryStr);
        query.bindValue(":ticker", ticker);

        if (!query.exec())
        {
            qDebug() << "Error fetching ticker id";
            closeDatabase();
            return DATABASE_ERROR;
        }

        if (query.next())
        {
            // Return the ticker id
            closeDatabase();
            return query.value(0).toInt();
        }

        closeDatabase();
        return NOT_FOUND;
    }

    qDebug() << "Error to open database to get ticker id";
    return DATABASE_ERROR;
}

int Database::getTransactionTypeId(TransactionType transactionType)
{
    if(openDatabase())
    {
        QString queryStr = R"(
            SELECT id FROM transaction_type_table WHERE transaction_type = :type;
        )";

        QSqlQuery query;
        query.prepare(queryStr);
        query.bindValue(":type", getTransactionTypeString(transactionType));

        if (!query.exec())
        {
            qDebug() << "Error fetching transaction type id";
            closeDatabase();
            return DATABASE_ERROR;
        }

        if (query.next())
        {
            // Return the id
            closeDatabase();
            return query.value(0).toInt();
        }

        closeDatabase();
        return NOT_FOUND;
    }

    qDebug() << "Error to open database to get transaction type id";
    return DATABASE_ERROR;
}

int Database::insertTicker(QString ticker, AssetType assetType)
{
    // Get asset id
    int assetTypeId = getAssetTypeId(assetType);

    // Check asset id
    if (assetTypeId == NOT_FOUND || assetTypeId == DATABASE_ERROR)
    {
        qDebug() << "Invalid asset type id, the database was loaded incorrect";
        return DATABASE_ERROR;
    }

    if(openDatabase())
    {
        QString insertQuery = R"(
            INSERT INTO ticker_table (ticker, id_asset_type)
            VALUES (:ticker, :id_asset_type);
        )";

        QSqlQuery query;
        query.prepare(insertQuery);
        query.bindValue(":ticker", ticker);
        query.bindValue(":id_asset_type", assetTypeId);

        // Execute query
        if (!query.exec())
        {
            qDebug() << "Error inserting ticker into ticker_table";
            closeDatabase();
            return DATABASE_ERROR;
        }

        closeDatabase();
        return DATABASE_SUCCESS;
    }

    qDebug() << "Error to open database to insert ticker";
    return DATABASE_ERROR;
}

bool Database::insertTransaction(QString ticker, AssetType assetType, Transaction transaction)
{
    // Convert to upper case
    ticker = ticker.toUpper();

    // Get transaction type id
    int transactionTypeId = getTransactionTypeId(transaction.getTransactionType());

    // Check transaction type id
    if (transactionTypeId == NOT_FOUND || transactionTypeId == DATABASE_ERROR)
    {
        qDebug() << "Invalid transaction type id, the database was loaded incorrect";
        return false;
    }

    int tickerId = getTickerId(ticker);

    // Check ticker status
    if(tickerId == DATABASE_ERROR)
    {
        return false;
    }

    // Check ticker id
    if(tickerId == NOT_FOUND)
    {
        // Inser new ticker
        int status = insertTicker(ticker, assetType);

        // Check status
        if(status == DATABASE_ERROR)
        {
            return false;
        }

        // Get ticker id
        tickerId = getTickerId(ticker);

        // Check ticker status
        if(tickerId == DATABASE_ERROR || tickerId == NOT_FOUND)
        {
            return false;
        }
    }

    if(openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL insert query
        query.prepare("INSERT INTO transaction_table (id_ticker, id_transaction, quantity, unitary_price, date) "
                      "VALUES (:id_ticker, :id_transaction, :quantity, :unitary_price, :date)");

        // Bind values to the query
        query.bindValue(":id_ticker", tickerId);
        query.bindValue(":id_transaction", transactionTypeId);
        query.bindValue(":quantity", transaction.getQuantity());
        query.bindValue(":unitary_price", transaction.getUnitaryPrice());
        query.bindValue(":date", transaction.getDate());

        // Execute the query and check for success
        if (!query.exec())
        {
            qDebug() << "Erro to insert new transaction";
            closeDatabase();
            return false;
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error to open database to insert transaction";
    return false;
}
