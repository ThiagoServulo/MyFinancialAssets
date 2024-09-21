#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>

class Database
{
public:
    Database();
    bool prepareDatabase();

private:
    QSqlDatabase database;
    bool openDatabase();
    void closeDatabase();
    bool createYieldTable();
};

#endif // DATABASE_H
