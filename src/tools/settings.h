#pragma once

#include "database.h"
#include "text.h"

class Settings
{
public:
    vector<std::pair<string, string>> defaults, data;
    map<string, vector<string>> options;
    string tableValues;
    const string name, columns, dataset;
    Database *db = nullptr;
    Table *table = nullptr;
    vector<Text *> texts;
    Settings(Database *db);
    string fetch(string key = "");
    ResultSet fetchAll();
    void fetchData();
};