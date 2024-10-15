#include "database.h"
#include "constants.h"
#include "basics.h"
#include "assetapi.h"
#include "databasecreation.h"
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

bool Database::checkIfDatabaseExists()
{
    // Get database path
    QString appDir = QCoreApplication::applicationDirPath();
    QString dbPath = appDir + "/database.db";
    QFile file(dbPath);

    // Check if exists
    return file.exists();
}

bool Database::prepareDatabase()
{
    DatabaseCreation databaseCreation;

    // Check if exists
    if(checkIfDatabaseExists())
    {
        return true;
    }

    // Create database
    if(openDatabase())
    {
        qDebug() << "Creating database";
        bool createStatus = databaseCreation.createDatabase();
        closeDatabase();
        return createStatus;
    }

    qDebug() << "Error to create database";
    return false ;
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

int Database::getYieldTypeId(YieldType yieldType)
{
    if(openDatabase())
    {
        QString queryStr = R"(
            SELECT id FROM yield_type_table WHERE yield_type = :type;
        )";

        QSqlQuery query;
        query.prepare(queryStr);
        query.bindValue(":type", getYieldTypeString(yieldType));

        if (!query.exec())
        {
            qDebug() << "Error fetching yield type id";
            closeDatabase();
            return DATABASE_ERROR;
        }

        if (query.next())
        {
            // Return the id
            closeDatabase();
            return query.value(0).toInt();
        }

        qDebug() << "Yield type not found";
        closeDatabase();
        return NOT_FOUND;
    }

    qDebug() << "Error to open database to get yield type id";
    return DATABASE_ERROR;
}

int Database::getReorganizationTypeId(ReorganizationType reorganizationType)
{
    if(openDatabase())
    {
        QString queryStr = R"(
            SELECT id FROM reorganization_type_table WHERE reorganization_type = :type;
        )";

        QSqlQuery query;
        query.prepare(queryStr);
        query.bindValue(":type", getReorganizationTypeString(reorganizationType));

        if (!query.exec())
        {
            qDebug() << "Error fetching reorganization type id";
            closeDatabase();
            return DATABASE_ERROR;
        }

        if (query.next())
        {
            // Return the id
            closeDatabase();
            return query.value(0).toInt();
        }

        qDebug() << "Reorganization type not found";
        closeDatabase();
        return NOT_FOUND;
    }

    qDebug() << "Error to open database to get reorganization type id";
    return DATABASE_ERROR;
}

int Database::getTickerId(QString ticker)
{
    // Convert to upper case
    ticker = ticker.toUpper();

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

int Database::insertTicker(QString ticker, AssetType assetType, double currentPrice)
{
    // Convert to upper case
    ticker = ticker.toUpper();

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
            INSERT INTO ticker_table (ticker, id_asset_type, current_price)
            VALUES (:ticker, :id_asset_type, :current_price);
        )";

        QSqlQuery query;
        query.prepare(insertQuery);
        query.bindValue(":ticker", ticker);
        query.bindValue(":id_asset_type", assetTypeId);
        query.bindValue(":current_price", currentPrice);

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
        // Get current price from API
        AssetApi assetApi;
        double currentPrice = assetApi.getAssetCurrentPrice(ticker);

        // Inser new ticker
        int status = insertTicker(ticker, assetType, currentPrice);

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
        query.prepare("INSERT INTO transaction_table (id_ticker, id_transaction_type, quantity, unitary_price, date) "
                      "VALUES (:id_ticker, :id_transaction_type, :quantity, :unitary_price, :date)");

        // Bind values to the query
        query.bindValue(":id_ticker", tickerId);
        query.bindValue(":id_transaction_type", transactionTypeId);
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

int Database::insertYield(QString ticker, Yield yield)
{
    // Convert to upper case
    ticker = ticker.toUpper();

    // Get yield type id
    int yieldTypeId = getYieldTypeId(yield.getYieldType());

    // Check yield type id
    if (yieldTypeId == NOT_FOUND || yieldTypeId == DATABASE_ERROR)
    {
        qDebug() << "Invalid yield type id, the database was loaded incorrect";
        return DATABASE_ERROR;
    }

    int tickerId = getTickerId(ticker);

    // Check ticker status
    if(tickerId == DATABASE_ERROR || tickerId == NOT_FOUND)
    {
        // Ticker should exists to insert the yield
        if(tickerId == NOT_FOUND)
        {
            qDebug() << "Ticker not found";
            return NOT_FOUND;
        }

        return DATABASE_ERROR;
    }

    if(openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL insert query
        query.prepare("INSERT INTO yield_table (id_ticker, id_yield_type, value, date) "
                      "VALUES (:id_ticker, :id_yield_type, :value, :date)");

        // Bind values to the query
        query.bindValue(":id_ticker", tickerId);
        query.bindValue(":id_yield_type", yieldTypeId);
        query.bindValue(":value", yield.getValue());
        query.bindValue(":date", yield.getDate());

        // Execute the query and check for success
        if (!query.exec())
        {
            qDebug() << "Erro to insert new yield";
            closeDatabase();
            return DATABASE_ERROR;
        }

        closeDatabase();
        return DATABASE_SUCCESS;
    }

    qDebug() << "Error to open database to insert yield";
    return DATABASE_ERROR;
}

int Database::insertReorganization(QString ticker, Reorganization reorganization)
{
    // Convert to upper case
    ticker = ticker.toUpper();

    // Get reorganization type id
    int reorganizationTypeId = getReorganizationTypeId(reorganization.getReorganizationType());

    // Check reorganization type id
    if (reorganizationTypeId == NOT_FOUND || reorganizationTypeId == DATABASE_ERROR)
    {
        qDebug() << "Invalid reorganization type id, the database was loaded incorrect";
        return DATABASE_ERROR;
    }

    int tickerId = getTickerId(ticker);

    // Check ticker status
    if(tickerId == DATABASE_ERROR || tickerId == NOT_FOUND)
    {
        // Ticker should exists to insert the yield
        if(tickerId == NOT_FOUND)
        {
            qDebug() << "Ticker not found";
            return NOT_FOUND;
        }

        return DATABASE_ERROR;
    }

    if(openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL insert query
        query.prepare("INSERT INTO reorganization_table (id_ticker, id_reorganization, ratio, date) "
                      "VALUES (:id_ticker, :id_reorganization, :ratio, :date)");

        // Bind values to the query
        query.bindValue(":id_ticker", tickerId);
        query.bindValue(":id_reorganization", reorganizationTypeId);
        query.bindValue(":ratio", reorganization.getRatio());
        query.bindValue(":date", reorganization.getDate());

        // Execute the query and check for success
        if (!query.exec())
        {
            qDebug() << "Erro to insert new reorganization";
            closeDatabase();
            return DATABASE_ERROR;
        }

        closeDatabase();
        return DATABASE_SUCCESS;
    }

    qDebug() << "Error to open database to insert reorganization";
    return DATABASE_ERROR;
}

int Database::getTickerQuantity(QString ticker)
{
    // Get ticker id
    int tickerId = getTickerId(ticker);

    // Check ticker id
    if(tickerId == NOT_FOUND || tickerId == DATABASE_ERROR)
    {
        return tickerId;
    }

    // Init quantity
    int quantity = 0;

    // Get events
    auto transactions = getTransactionsByTickerId(tickerId);
    auto reorganizations = getReorganizationsByTickerId(tickerId);
    auto events = mergeAndSortEvents(transactions, reorganizations);

    for(auto event: events)
    {
        if(event->getEventType() == EventType::REORGANIZATION)
        {
            // Cast to reorganization
            Reorganization* reorganization = dynamic_cast<Reorganization*>(event);

            // Check cast
            if (reorganization)
            {
                // Check reorganization type
                if(reorganization->getReorganizationType() == ReorganizationType::GRUPAMENTO)
                {
                    quantity = (quantity == 0) ? 0 : (quantity / reorganization->getRatio());
                }
                else if (reorganization->getReorganizationType() == ReorganizationType::DESDOBRAMENTO)
                {
                    quantity = static_cast<int>(quantity * reorganization->getRatio());
                }
                else
                {
                    throw std::invalid_argument("Reorganization type invalid");
                }
            }
        }
        else if(event->getEventType() == EventType::TRANSACTION)
        {
            // Cast to transaction
            Transaction* transaction = dynamic_cast<Transaction*>(event);

            // Check cast
            if (transaction)
            {
                // Check transaction type
                if(transaction->getTransactionType() == TransactionType::COMPRA)
                {
                    quantity += transaction->getQuantity();
                }
                else if(transaction->getTransactionType() == TransactionType::VENDA)
                {
                    quantity -= transaction->getQuantity();
                }
                else
                {
                    throw std::invalid_argument("Transaction type invalid");
                }
            }
        }
    }

    // Return quantity
    return quantity;
}

std::vector<Transaction> Database::getTransactionsByTickerId(int tickerId)
{
    std::vector<Transaction> transactions;

    if (openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL select query with JOIN
        query.prepare(R"(
            SELECT tt.transaction_type, t.quantity, t.unitary_price, t.date
            FROM transaction_table t
            JOIN transaction_type_table tt ON t.id_transaction_type = tt.id
            WHERE t.id_ticker = :id_ticker;
        )");

        // Bind the tickerId to the query
        query.bindValue(":id_ticker", tickerId);

        // Execute the query
        if (!query.exec())
        {
            qDebug() << "Error retrieving transactions for tickerId:" << tickerId;
            closeDatabase();

             // Returning an empty vector in case of error
            return transactions;
        }

        // Iterate over the results and populate the vector
        while (query.next())
        {
            QString transactionTypeStr = query.value(0).toString();
            int quantity = query.value(1).toInt();
            double unitaryPrice = query.value(2).toDouble();
            QString date = query.value(3).toString();

            // Convert transaction type string to TransactionType
            TransactionType transactionType = getTransactionTypeFromString(transactionTypeStr);

            // Create a Transaction object
            Transaction transaction( QDate::fromString(date, "yyyy-MM-dd"), transactionType, quantity, unitaryPrice);

            // Add the transaction to the vector
            transactions.push_back(transaction);
        }

        closeDatabase();
    }
    else
    {
        qDebug() << "Error opening database to retrieve transactions";
    }

    return transactions;
}

bool Database::selectAllAssets(std::vector<Asset>& assets)
{
    if (openDatabase())
    {
        QString selectQuery = R"(
            SELECT ticker, asset_type, current_price
            FROM ticker_table
            JOIN asset_type_table ON ticker_table.id_asset_type = asset_type_table.id;
        )";

        QSqlQuery query;
        query.prepare(selectQuery);

        // Execute query
        if (!query.exec())
        {
            qDebug() << "Error selecting tickers from ticker_table";
            closeDatabase();
            return false;
        }

        // Clear the vector to avoid overwriting old data
        assets.clear();

        // Process the results
        while (query.next())
        {
            QString ticker = query.value(0).toString();
            QString assetTypeStr = query.value(1).toString();
            double currentPrice = query.value(2).toDouble();

            // Convert assetTypeStr to AssetType
            AssetType assetType = getAssetTypeFromString(assetTypeStr);
            assets.push_back(Asset(ticker, assetType, currentPrice));
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error opening database to select tickers";
    return false;
}

bool Database::selectEventsForAsset(Asset* asset)
{
    // Create events vetor
    std::vector<std::shared_ptr<Event>> events;

    // Get ticker id
    int tickerId = getTickerId(asset->getTicker());

    // Check ticker id
    if(tickerId == DATABASE_ERROR || tickerId == NOT_FOUND)
    {
        return false;
    }

    // Get transactions
    std::vector<Transaction> transactions = getTransactionsByTickerId(tickerId);

    // Convert transactions to events
    for (const auto& transaction : transactions)
    {
        events.push_back(std::make_shared<Transaction>(transaction));
    }

    // Add events
    asset->addEvents(events);

    return true;
}

bool Database::investmentcontrollerInitialization(InvestmentController* investmentcontroller)
{
    std::vector<Asset> assets;

    if(checkLastUpdate())
    {
        // Update current price
        QStringList tickers;
        if(selectAllTicker(&tickers))
        {
            for(const QString &ticker: tickers)
            {
                AssetApi assetApi;
                double currentPrice = assetApi.getAssetCurrentPrice(ticker);
                updateTickerCurrentPrice(ticker, currentPrice);
            }

            // Update last update date
            insertLastUpdateDate(QDate::currentDate());
        }
    }

    // Get assets
    if(!selectAllAssets(assets))
    {
        qDebug() << "Error to read assets from database";
        return false;
    }

    // Get events
    for(Asset asset: assets)
    {
        // Select events
        selectEventsForAsset(&asset);
    }

    // Adding assets
    investmentcontroller->addAssets(assets);

    return true;
}

double Database::getTickerAveragePrice(QString ticker)
{
    // Get ticker id
    int tickerId = getTickerId(ticker);

    // Check ticker id
    if(tickerId == NOT_FOUND || tickerId == DATABASE_ERROR)
    {
        return tickerId;
    }

    // Init variables
    double total = 0;
    int quantity = 0;

    // Get transactions
    std::vector<Transaction> transactions = getTransactionsByTickerId(tickerId);

    for(auto transaction: transactions)
    {
        // Check transaction type
        if(transaction.getTransactionType() == TransactionType::COMPRA)
        {
            total += (transaction.getUnitaryPrice() * transaction.getQuantity());
            quantity += transaction.getQuantity();
        }
    }

    // Return average price
    return total/quantity;
}

std::vector<Yield> Database::getYieldsByTickerId(int tickerId)
{
    std::vector<Yield> yields;

    if (openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL select query with JOIN
        query.prepare(R"(
            SELECT yt.yield_type, y.value, y.date
            FROM yield_table y
            JOIN yield_type_table yt ON y.id_yield_type = yt.id
            WHERE y.id_ticker = :id_ticker;
        )");

        // Bind the tickerId to the query
        query.bindValue(":id_ticker", tickerId);

        // Execute the query
        if (!query.exec())
        {
            qDebug() << "Error retrieving yields for tickerId:" << tickerId;
            closeDatabase();

              // Returning an empty vector in case of error
            return yields;
        }

        // Iterate over the results and populate the vector
        while (query.next())
        {
            QString yieldTypeStr = query.value(0).toString();
            double value = query.value(1).toDouble();
            QString date = query.value(2).toString();

            // Convert yield type string to YieldType
            YieldType yieldType = getYieldTypeFromString(yieldTypeStr);

            // Create a Yield object
            Yield yield(QDate::fromString(date, "yyyy-MM-dd"), yieldType, value);

            // Add the yield to the vector
            yields.push_back(yield);
        }

        closeDatabase();
    }
    else
    {
        qDebug() << "Error opening database to retrieve yields";
    }

    return yields;
}

std::vector<Reorganization> Database::getReorganizationsByTickerId(int tickerId)
{
    std::vector<Reorganization> reorganizations;

    if (openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL select query with JOIN
        query.prepare(R"(
            SELECT rt.reorganization_type, r.ratio, r.date
            FROM reorganization_table r
            JOIN reorganization_type_table rt ON r.id_reorganization = rt.id
            WHERE r.id_ticker = :id_ticker;
        )");

        // Bind the tickerId to the query
        query.bindValue(":id_ticker", tickerId);

        // Execute the query
        if (!query.exec())
        {
            qDebug() << "Error retrieving reorganizations for tickerId:" << tickerId;
            closeDatabase();

              // Returning an empty vector in case of error
            return reorganizations;
        }

        // Iterate over the results and populate the vector
        while (query.next())
        {
            QString reorganizationTypeStr = query.value(0).toString();
            int ratio = query.value(1).toDouble();
            QString date = query.value(2).toString();

            // Convert reorganization type string to ReorganizationType
            ReorganizationType reorganizationType = getReorganizationTypeFromString(reorganizationTypeStr);

            // Create a reorganization object
            Reorganization reorganization(QDate::fromString(date, "yyyy-MM-dd"), reorganizationType, ratio);

            // Add the reorganization to the vector
            reorganizations.push_back(reorganization);
        }

        closeDatabase();
    }
    else
    {
        qDebug() << "Error opening database to retrieve yields";
    }

    return reorganizations;
}

double Database::getTickerTotalYield(QString ticker)
{
    // Get ticker id
    int tickerId = getTickerId(ticker);

    // Check ticker id
    if(tickerId == NOT_FOUND || tickerId == DATABASE_ERROR)
    {
        return tickerId;
    }

    // Init variables
    double total = 0;

    // Get yields
    std::vector<Yield> yields = getYieldsByTickerId(tickerId);

    for(auto yield: yields)
    {
        total += yield.getValue();
    }

    // Return total
    return total;
}

std::vector<Transaction> Database::getTickerTransactions(QString ticker)
{
    // Init transactions
    std::vector<Transaction> transactions;

    // Get ticker id
    int tickerId = getTickerId(ticker);

    // Check ticker id
    if(tickerId == NOT_FOUND || tickerId == DATABASE_ERROR)
    {
        return transactions;
    }

    // Get transactions
    transactions = getTransactionsByTickerId(tickerId);

    // Return transactions
    return transactions;
}

std::vector<Yield> Database::getTickerYields(QString ticker)
{
    // Init yields
    std::vector<Yield> yields;

    // Get ticker id
    int tickerId = getTickerId(ticker);

    // Check ticker id
    if(tickerId == NOT_FOUND || tickerId == DATABASE_ERROR)
    {
        return yields;
    }

    // Get yields
    yields = getYieldsByTickerId(tickerId);

    // Return yields
    return yields;
}

std::vector<Reorganization> Database::getTickerReorganizations(QString ticker)
{
    // Init reorganizations
    std::vector<Reorganization> reorganizations;

    // Get ticker id
    int tickerId = getTickerId(ticker);

    // Check ticker id
    if(tickerId == NOT_FOUND || tickerId == DATABASE_ERROR)
    {
        return reorganizations;
    }

    // Get reorganizations
    reorganizations = getReorganizationsByTickerId(tickerId);

    // Return reorganizations
    return reorganizations;
}

QDate Database::selectLastUpdateDate()
{
    if (openDatabase())
    {
        // Query to get last update
        QString selectQuery = R"(
                SELECT date FROM last_update_table;
        )";

        // Execute query
        QSqlQuery query;
        if (!query.exec(selectQuery))
        {
            qDebug() << "Error to get last update date";
            closeDatabase();
            return QDate(0, 0, 0);
        }

        query.next();
        QString date = query.value(0).toString();
        closeDatabase();
        return QDate::fromString(date, "yyyy-MM-dd");
    }

    qDebug() << "Error opening database to select last update date";
    return QDate(0, 0, 0);
}

bool Database::checkLastUpdate()
{
    QDate lastUpdateDate = selectLastUpdateDate();
    QDate currentDate = QDate::currentDate();

    return lastUpdateDate < currentDate;
}

bool Database::selectAllTicker(QStringList *tickers)
{
    if (openDatabase())
    {
        QString selectQuery = R"(
            SELECT ticker
            FROM ticker_table;
        )";

        QSqlQuery query;
        query.prepare(selectQuery);

        // Execute query
        if (!query.exec())
        {
            qDebug() << "Error selecting tickers from ticker_table";
            closeDatabase();
            return false;
        }

        // Clear string list
        tickers->clear();

        // Process the results
        while (query.next())
        {
            tickers->append(query.value(0).toString());
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error opening database to select tickers";
    return false;
}

int Database::updateTickerCurrentPrice(QString ticker, double currentPrice)
{
    // Convert to upper case
    ticker = ticker.toUpper();

    if(openDatabase())
    {
        QString insertQuery = R"(
            UPDATE ticker_table SET current_price = :current_price WHERE ticker = :ticker;
        )";

        QSqlQuery query;
        query.prepare(insertQuery);
        query.bindValue(":ticker", ticker);
        query.bindValue(":current_price", currentPrice);

        // Execute query
        if (!query.exec())
        {
            qDebug() << "Error updating current price into ticker_table";
            closeDatabase();
            return DATABASE_ERROR;
        }

        closeDatabase();
        return DATABASE_SUCCESS;
    }

    qDebug() << "Error to open database to update current price";
    return DATABASE_ERROR;
}

int Database::insertLastUpdateDate(QDate date)
{
    QSqlQuery query;

    if(openDatabase())
    {
        // Clear the table
        QString deleteQuery = "DELETE FROM last_update_table;";

        if (!query.exec(deleteQuery))
        {
            qDebug() << "Error deleting rows from last_update_table";
            closeDatabase();
            return DATABASE_ERROR;
        }

        // Build query to update table
        QString insertQuery = R"(
            INSERT INTO last_update_table (date)
            VALUES (:date);
        )";

        query.prepare(insertQuery);
        query.bindValue(":date", date.toString("yyyy-MM-dd"));

        // Execute query
        if (!query.exec())
        {
            qDebug() << "Error inserting last update date";
            closeDatabase();
            return DATABASE_ERROR;
        }

        closeDatabase();
        return DATABASE_SUCCESS;
    }

    qDebug() << "Error to open database to insert last update date";
    return DATABASE_ERROR;
}

int Database::insertFixedIncome(FixedIncome fixedIncome)
{
    QSqlQuery query;
/*
    if(openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL insert query
        query.prepare("INSERT INTO fixed_income_table (desciption, yield_expected, purchase_date, limit_date, "
                      "invested_value, current_value, status) "
                      "VALUES (:desciption, :yield_expected, :purchase_date, :limit_date, :invested_value, "
                      ":current_value ,:status)");

        // Bind values to the query
        query.bindValue(":desciption", fixedIncome);
        query.bindValue(":yield_expected", reorganizationTypeId);
        query.bindValue(":purchase_date", reorganization.getRatio());
        query.bindValue(":limit_date", reorganization.getDate());
        query.bindValue(":invested_value", reorganizationTypeId);
        query.bindValue(":current_value", reorganization.getRatio());
        query.bindValue(":status", reorganization.getDate())

        // Execute the query and check for success
        if (!query.exec())
        {
            qDebug() << "Erro to insert new reorganization";
            closeDatabase();
            return DATABASE_ERROR;
        }

        closeDatabase();
        return DATABASE_SUCCESS;
    }
    */
}
