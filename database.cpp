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

bool Database::insertAsset(Asset asset)
{
    if(openDatabase())
    {
        QString insertQuery = R"(
            INSERT INTO ticker_table (ticker, id_asset_type, current_price)
            VALUES (:ticker, (SELECT id FROM asset_type_table WHERE asset_type = :asset_type), :current_price);
        )";

        QSqlQuery query;
        query.prepare(insertQuery);
        query.bindValue(":ticker", asset.getTicker());
        query.bindValue(":asset_type", getAssetTypeString(asset.getAssetType()));
        query.bindValue(":current_price", asset.getCurrentPrice());

        // Execute query
        if (!query.exec())
        {
            qDebug() << "Error inserting ticker into ticker_table";
            closeDatabase();
            return false;
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error to open database to insert ticker";
    return false;
}

bool Database::insertTransaction(QString ticker, Transaction transaction)
{
    if(openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL insert query
        query.prepare("INSERT INTO transaction_table (id_ticker, id_transaction_type, quantity, unitary_price, date) "
                      "VALUES ((SELECT id FROM ticker_table WHERE ticker = :ticker), "
                      "(SELECT id FROM transaction_type_table WHERE transaction_type = :transaction_type), "
                      ":quantity, :unitary_price, :date)");

        // Bind values to the query
        query.bindValue(":ticker", ticker);
        query.bindValue(":transaction_type", getTransactionTypeString(transaction.getTransactionType()));
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

bool Database::insertYield(QString ticker, Yield yield)
{
    if(openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL insert query
        query.prepare("INSERT INTO yield_table (id_ticker, id_yield_type, value, date) "
                      "VALUES ((SELECT id FROM ticker_table WHERE ticker = :ticker), "
                      "(SELECT id FROM yield_type_table WHERE yield_type = :yield_type), :value, :date)");

        // Bind values to the query
        query.bindValue(":ticker", ticker.toUpper());
        query.bindValue(":yield_type", getYieldTypeString(yield.getYieldType()));
        query.bindValue(":value", yield.getValue());
        query.bindValue(":date", yield.getDate());

        // Execute the query and check for success
        if (!query.exec())
        {
            qDebug() << "Erro to insert new yield";
            closeDatabase();
            return false;
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error to open database to insert yield";
    return false;
}

bool Database::insertReorganization(QString ticker, Reorganization reorganization)
{
    if(openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL insert query
        query.prepare(
            "INSERT INTO reorganization_table (id_ticker, id_reorganization, ratio, date) "
            "VALUES ((SELECT id FROM ticker_table WHERE ticker = :ticker), "
            "(SELECT id FROM reorganization_type_table WHERE reorganization_type = :reorganization_type), :ratio, :date)"
        );

        // Bind values to the query
        query.bindValue(":ticker", ticker.toUpper());
        query.bindValue(":reorganization_type", getReorganizationTypeString(reorganization.getReorganizationType()));
        query.bindValue(":ratio", reorganization.getRatio());
        query.bindValue(":date", reorganization.getDate());

        // Execute the query and check for success
        if (!query.exec())
        {
            qDebug() << "Erro to insert new reorganization";
            closeDatabase();
            return false;
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error to open database to insert reorganization";
    return false;
}

std::vector<Transaction> Database::getTransactionsByTicker(QString ticker)
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
            WHERE t.id_ticker = (SELECT id FROM ticker_table WHERE ticker = :ticker);
        )");

        // Bind the tickerId to the query
        query.bindValue(":ticker", ticker);

        // Execute the query
        if (!query.exec())
        {
            qDebug() << "Error retrieving transactions for ticker: " << ticker;
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
            Transaction transaction(QDate::fromString(date, "yyyy-MM-dd"), transactionType, quantity, unitaryPrice);

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
    // Get transactions
    std::vector<Transaction> transactions = getTransactionsByTicker(asset->getTicker());

    // Convert transactions to events
    for (const auto& transaction : transactions)
    {
        asset->addEvent(std::make_shared<Transaction>(transaction));
    }

    // Get yields
    std::vector<Yield> yields = getYieldsByTicker(asset->getTicker());

    // Convert yields to events
    for (const auto& yield : yields)
    {
        asset->addEvent(std::make_shared<Yield>(yield));
    }

    // Get reorganizations
    std::vector<Reorganization> reorganizations = getReorganizationsByTicker(asset->getTicker());

    // Convert reorganizations to events
    for (const auto& reorganization : reorganizations)
    {
        asset->addEvent(std::make_shared<Reorganization>(reorganization));
    }

    return true;
}

bool Database::investmentControllerInitialization(InvestmentController* investmentController)
{
    std::vector<Asset> assets;
    AssetApi assetApi;

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

        // Add asset
        investmentController->addAsset(std::make_shared<Asset>(asset));

        if(checkLastUpdate() && asset.getQuantity(nullptr, nullptr) > 0)
        {
            asset.setCurrentPrice(assetApi.getAssetCurrentPrice(asset.getTicker()));
            updateTickerCurrentPrice(asset.getTicker(), asset.getCurrentPrice());
        }
    }

    insertLastUpdateDate(QDate::currentDate());

    // Get fixed incomes
    for(auto fixedIncome : selectAllFixedIncomes())
    {
        investmentController->addFixedIncome(std::make_shared<FixedIncome>(fixedIncome));
    }

    // Get financial institutions
    for(auto financialInstitution : selectAllFinancialInstitutions())
    {
        investmentController->addFinancialInstitution(std::make_shared<FinancialInstitution>(financialInstitution));
    }

    return true;
}

std::vector<Yield> Database::getYieldsByTicker(QString ticker)
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
            WHERE y.id_ticker = (SELECT id FROM ticker_table WHERE ticker = :ticker);
        )");

        // Bind the ticker to the query
        query.bindValue(":ticker", ticker);

        // Execute the query
        if (!query.exec())
        {
            qDebug() << "Error retrieving yields for ticker: " << ticker;
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

std::vector<Reorganization> Database::getReorganizationsByTicker(QString ticker)
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
            WHERE r.id_ticker =  (SELECT id FROM ticker_table WHERE ticker = :ticker);
        )");

        // Bind the ticker to the query
        query.bindValue(":ticker", ticker);

        // Execute the query
        if (!query.exec())
        {
            qDebug() << "Error retrieving reorganizations for ticker: " << ticker;
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

bool Database::updateTickerCurrentPrice(QString ticker, double currentPrice)
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
            return false;
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error to open database to update current price";
    return false;
}

bool Database::insertLastUpdateDate(QDate date)
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
            return false;
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
            return false;
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error to open database to insert last update date";
    return false;
}

bool Database::insertFixedIncome(FixedIncome fixedIncome)
{
    if(openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL insert query
        query.prepare("INSERT INTO fixed_income_table (description, yield_expected, purchase_date, limit_date, "
                      "invested_value, current_value, status) "
                      "VALUES (:description, :yield_expected, :purchase_date, :limit_date, :invested_value, "
                      ":current_value ,:status)");

        // Bind values to the query
        query.bindValue(":description", fixedIncome.getDescription());
        query.bindValue(":yield_expected", fixedIncome.getYieldExpected());
        query.bindValue(":purchase_date", fixedIncome.getPurchaseDate());
        query.bindValue(":limit_date", fixedIncome.getLimitDate());
        query.bindValue(":invested_value", fixedIncome.getInvestedValue());
        query.bindValue(":current_value", fixedIncome.getCurrentValue());
        query.bindValue(":status", fixedIncome.getStatus());

        // Execute the query and check for success
        if (!query.exec())
        {
            qDebug() << "Erro to insert new fixed income";
            closeDatabase();
            return false;
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error to open database to insert fixed income";
    return false;
}

std::vector<FixedIncome> Database::selectAllFixedIncomes()
{
    std::vector<FixedIncome> fixedIncomes;

    if (openDatabase())
    {
        QString selectQuery = R"(
            SELECT description, yield_expected, purchase_date, limit_date, invested_value, current_value, status
            FROM fixed_income_table;
        )";

        QSqlQuery query;
        query.prepare(selectQuery);

        // Execute query
        if (!query.exec())
        {
            qDebug() << "Error selecting tickers from fixed_income_table";
            closeDatabase();
            return fixedIncomes;
        }

        // Clear the vector
        fixedIncomes.clear();

        // Process the results
        while (query.next())
        {
            QString description = query.value(0).toString();
            QString yieldExpected = query.value(1).toString();
            QDate purchaseDate = QDate::fromString(query.value(2).toString(), "yyyy-MM-dd");
            QDate limitDate = QDate::fromString(query.value(3).toString(), "yyyy-MM-dd");
            double investedValue = query.value(4).toDouble();
            double currentValue = query.value(5).toDouble();
            bool status = query.value(6).toBool();

            fixedIncomes.push_back(FixedIncome(purchaseDate, description, yieldExpected, investedValue, limitDate, currentValue, status));
        }

        closeDatabase();
        return fixedIncomes;
    }
    else
    {
        qDebug() << "Error opening database to select fixed incomes";
    }

    return fixedIncomes;
}

bool Database::updateFixedIncome(FixedIncome *fixedIncome)
{
    if(openDatabase())
    {
        // Prepare the SQL update query
        QString updateQuery = R"(
            UPDATE fixed_income_table
            SET status = :status, current_value = :current_value
            WHERE purchase_date = :purchase_date
              AND description = :description
              AND limit_date = :limit_date;
        )";

        // Create query
        QSqlQuery query;
        query.prepare(updateQuery);

        // Bind values to the query
        query.bindValue(":status", fixedIncome->getStatus());
        query.bindValue(":current_value", fixedIncome->getCurrentValue());
        query.bindValue(":purchase_date", fixedIncome->getPurchaseDate());
        query.bindValue(":description", fixedIncome->getDescription());
        query.bindValue(":limit_date", fixedIncome->getLimitDate());

        // Execute the query and check for success
        if (!query.exec())
        {
            qDebug() << "Erro to update fixed income";
            closeDatabase();
            return false;
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error to open database to update fixed income";
    return false;
}

bool Database::insertFinancialInstitution(FinancialInstitution financialInstitution)
{
    if(openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL insert query
        query.prepare("INSERT INTO financial_institution_table (name) VALUES (:name)");

        // Bind values to the query
        query.bindValue(":name", financialInstitution.getName());

        // Execute the query and check for success
        if (!query.exec())
        {
            qDebug() << "Erro to insert new financial institution";
            closeDatabase();
            return false;
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error to open database to insert financial institution";
    return false;
}

bool Database::insertFinancialInstitutionMonth(QString name, FinancialInstitutionMonth result)
{
    if(openDatabase())
    {
        QSqlQuery query;

        // Prepare the SQL insert query
        query.prepare("INSERT INTO financial_institution_month_table (id_financial_institution, value, date) "
                      "VALUES ((SELECT id FROM financial_institution_table WHERE name = :name), :value, :date)");

        // Bind values to the query
        query.bindValue(":name", name);
        query.bindValue(":value", result.getValue());
        query.bindValue(":date", result.getDate());

        // Execute the query and check for success
        if (!query.exec())
        {
            qDebug() << "Erro to insert new financial institution month";
            closeDatabase();
            return false;
        }

        closeDatabase();
        return true;
    }

    qDebug() << "Error to open database to insert financial institution";
    return false;
}

std::vector<FinancialInstitution> Database::selectAllFinancialInstitutions()
{
    std::vector<FinancialInstitution> financialInstitutions;

    if (openDatabase())
    {
        QString selectQuery1 = R"(
            SELECT id, name FROM financial_institution_table;
        )";

        QSqlQuery query1;
        query1.prepare(selectQuery1);

        // Execute query
        if (!query1.exec())
        {
            qDebug() << "Error selecting financial institutions from financial_institution_table";
            closeDatabase();
            return financialInstitutions;
        }

        // Clear the vector
        financialInstitutions.clear();

        // Process the results
        while (query1.next())
        {
            int id_financial_institution = query1.value(0).toString().toInt();
            QString name = query1.value(1).toString();

            FinancialInstitution financialInstitution = FinancialInstitution(name);

            QSqlQuery query2;

            // Prepare the SQL insert query
            query2.prepare("SELECT value, date FROM financial_institution_month_table WHERE id_financial_institution = :id");

            // Bind values to the query
            query2.bindValue(":id", id_financial_institution);

            // Execute query
            if (!query2.exec())
            {
                qDebug() << "Error selecting financial institutions month from financial_institution_month_table";
                continue;
            }

            // Process the results
            while (query2.next())
            {
                double value = query2.value(0).toString().toDouble();
                QDate date = QDate::fromString(query2.value(1).toString(), "yyyy-MM-dd");

                financialInstitution.addFinancialResult(std::make_shared<FinancialInstitutionMonth>(date, value));
            }

            financialInstitutions.push_back(financialInstitution);
        }

        closeDatabase();
        return financialInstitutions;
    }
    else
    {
        qDebug() << "Error opening database to select fixed incomes";
    }

    return financialInstitutions;
}

bool Database::deleteTransaction(QString ticker, TransactionType type, QDate date, int quantity, double price)
{
    if (openDatabase())
    {
        QSqlQuery query;

        // Get id_ticker
        query.prepare("SELECT id FROM ticker_table WHERE ticker = :ticker");
        query.bindValue(":ticker", ticker);

        if (!query.exec() || !query.next())
        {
            qDebug() << "Erro getting ticker id";
            return false;
        }

        int id_ticker = query.value(0).toInt();

        // Prepare the query
        query.prepare(R"(
            DELETE FROM transaction_table
            WHERE id_ticker = :id_ticker
            AND id_transaction_type = :id_transaction_type
            AND date = :date
            AND quantity = :quantity
            AND unitary_price = :price
        )");

        query.bindValue(":id_ticker", id_ticker);
        query.bindValue(":id_transaction_type", static_cast<int>(type));
        query.bindValue(":date", date.toString("yyyy-MM-dd"));
        query.bindValue(":quantity", quantity);
        query.bindValue(":price", price);

        // Execute query
        if (!query.exec())
        {
            qDebug() << "Error deleting transaction from transaction_table";
            return false;
        }

        return true;
    }
    else
    {
        qDebug() << "Error opening database to delete transaction";
        return false;
    }
}
