#include "../database.h"

Database::Database(string source)
{
    if (sqlite3_open(("data/" + source + ".db").c_str(), &db) != 0)
        log("Database Uninitialized: " + source);  
}

void Database::createTable(string name, string columns)
{
    Table *table = new Table(db, name, columns);
    tables[name] = table;
}

Table *Database::findTable(string name)
{
    return tables[name];
}