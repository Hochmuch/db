#include "table.h"
#include <stdexcept>
#include <iostream>

void Table::addColumn(const ColumnDefinition &column) {
    std::cout << "Adding column: " << column.name << std::endl;
    columns.push_back(column);
    columnIndex[column.name] = columns.size() - 1;
    for (auto &row : rows) {
        row.push_back(column.defaultValue);
    }
}

const std::vector<ColumnDefinition>& Table::getColumns() const {
    return columns;
}

void Table::insertRow(const std::vector<std::pair<std::string, DataType>> &row) {
    Row newRow(columns.size(), DataType{});
    for (const auto &pair : row) {
        const std::string &name = pair.first;
        const DataType &value = pair.second;
        auto columnIter = columnIndex.find(name);
        if (columnIter == columnIndex.end()) {
            throw std::runtime_error("Column not found: " + name);
        }
        size_t index = columnIter->second;

        const ColumnDefinition &columnDef = columns[index];
        if (columnDef.type == "string" && columnDef.length != -1) {
            if (std::holds_alternative<std::string>(value)) {
                std::string strVal = std::get<std::string>(value);
                if (strVal.size() > columnDef.length) {
                    throw std::runtime_error("String length exceeds defined length for column: " + name);
                }
            }
        }

        newRow[index] = value;
    }
    std::cout << "Row inserted with size: " << newRow.size() << std::endl;
    rows.push_back(newRow);
}

std::vector<Row> Table::selectRows() const {
    return rows;
}

void Table::updateRows(const std::map<std::string, DataType> &updates) {
    for (auto &row : rows) {
        for (const auto &update : updates) {
            auto columnIter = columnIndex.find(update.first);
            if (columnIter == columnIndex.end()) {
                throw std::runtime_error("Column not found: " + update.first);
            }
            size_t index = columnIter->second;
            row[index] = update.second;
        }
    }
}

void Table::deleteRows() {
    rows.clear();
}