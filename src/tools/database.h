#pragma once

#include "../core/config.h"
#include "table.h"

class Database
{
public:
    sqlite3 *db = nullptr;
    unordered_map<string, Table *> tables;
    Database(string source);
    Table *createTable(string name, string columns);
    Table *findTable(string name);
};