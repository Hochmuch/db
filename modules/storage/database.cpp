#include "database.h"
#include "../language/utils.h"
#include <stdexcept>
#include <iostream>

void Database::createTable(const std::string &name, const std::vector<ColumnDefinition>& columns) {
    std::string tableName = name;
    trim(tableName);
    trim(tableName);

    if (tables.find(tableName) != tables.end()) {
        throw std::runtime_error("Table already exists: " + tableName);
    }

    Table table;
    for (const auto &column : columns) {
        table.addColumn(column);
    }

    tables[tableName] = table;
    std::cout << "Table created: \"" << tableName << "\"" << std::endl;
}

void Database::insertIntoTable(const std::string &tableName, const std::vector<std::pair<std::string, DataType>>& row) {
    std::string trimmedTableName = tableName;
    trim(trimmedTableName);

    auto tableIterator = tables.find(trimmedTableName);
    if (tableIterator == tables.end()) {
        throw std::runtime_error("Table not found: " + trimmedTableName);
    }
    Table &table = tableIterator->second;

    std::cout << "Inserting into table: \"" << trimmedTableName << "\"" << std::endl;
    table.insertRow(row);
}

std::vector<Row> Database::selectFromTable(const std::string &tableName) const {
    std::string trimmedTableName = tableName;
    trim(trimmedTableName);

    auto tableIterator = tables.find(trimmedTableName);
    if (tableIterator == tables.end()) {
        throw std::runtime_error("Table not found: " + trimmedTableName);
    }
    return tableIterator->second.selectRows();
}

const std::map<std::string, Table>& Database::getTables() const {
    return tables;
}