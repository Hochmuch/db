#pragma once
#include <string>
#include <vector>
#include <map>
#include "table.h"

class Database {
public:
    std::map<std::string, Table> tables;

    void createTable(const std::string &name, const std::vector<ColumnDefinition>& columns);
    void insertIntoTable(const std::string &tableName, const std::vector<std::pair<std::string, DataType>>& row);
    [[nodiscard]] std::vector<Row> selectFromTable(const std::string &tableName) const;

    [[nodiscard]] const std::map<std::string, Table>& getTables() const;
};