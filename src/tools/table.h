#pragma once

#include "../core/config.h"

using Row = unordered_map<string, string>;
using ResultSet = vector<Row>;

class Table
{
public:
    sqlite3 *db = nullptr;
    string name;
    Table(sqlite3 *db, string name, string columns);
    int insert(string dataset, string values);
    int drop();
    ResultSet select(string what, string where = "");
private:
    static int selectCallback(void *data, int argc, char **argv, char **colNames);
};