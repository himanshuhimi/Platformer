#include "../settings.h"

Settings::Settings(Database *db) : db(db)
{
    options = {
        {
            "window",
            {
                "640x360",
                "1280x720",
                "1920x1080"
            }
        },
    };
    vector<std::pair<string, string>> defaults = {
        {"window", "640x360"},
        {"Move Forward", "D"},
        {"Move Backward", "A"},
        {"Jump", "Space"}
    };
    string name = "settings";
    string dataset = "key, value";
    string columns = "key TEXT PRIMARY KEY, value TEXT NOT NULL";
    bool first = true;
    for (auto &[key, value] : defaults)
    {
        if (!first)
            tableValues += ",";
        tableValues += "('" + key + "','" + value + "')" ;
        first = false;
    }
    table = db->createTable(name, columns);
    table->insert(dataset, tableValues);
    fetchData();
}

string Settings::fetch(string key)
{
    return table->select("*", "key = '" + key + "'")[0]["value"];
}

ResultSet Settings::fetchAll()
{
    return table->select("*");
}

void Settings::fetchData()
{
    data.clear();
    ResultSet currentData = fetchAll();
    for (Row row : currentData)
        data.push_back({row["key"], row["value"]});
}