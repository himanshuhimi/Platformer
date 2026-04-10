#include "../table.h"

Table::Table(sqlite3 *db, string name, string columns) : db(db), name(name) 
{
    string sql = "CREATE TABLE IF NOT EXISTS " + name + "(" + columns + ")";
    sqlite3_exec(db, sql.c_str(), 0, 0, 0);
}

int Table::insert(string what, string values)
{
    string sql = "INSERT INTO " + name + " " + what + " VALUES " + values;
    return sqlite3_exec(db, sql.c_str(), 0, 0, 0);
}

int Table::drop()
{
    string sql = "DROP TABLE " + name;
    return sqlite3_exec(db, sql.c_str(), 0, 0, 0);
}

ResultSet Table::select(string what, string where)
{
    ResultSet results;
    string sql = "SELECT " + what + " FROM " + name;
    if (!where.empty())
        sql += " WHERE " + where;
    sqlite3_exec(db, sql.c_str(), selectCallback, &results, nullptr);
    return results;
};

int Table::selectCallback(void *data, int argc, char **argv, char **colNames)
{
    ResultSet *results = static_cast<ResultSet*>(data);
    Row row;
    for (int i = 0; i < argc; i++)
    {
        string key = colNames[i];
        string value = argv[i] ? argv[i] : "NULL";
        row[key] = value;
    }
    results->push_back(row);
    return 0;
}