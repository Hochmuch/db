#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <variant>

using DataType = std::variant<int32_t, bool, std::string, std::vector<std::byte>>;
using Row = std::vector<DataType>;

enum class ColumnAttribute {
    UNIQUE,
    AUTOINCREMENT
};

struct ColumnDefinition {
    std::string name;
    std::string type;
    bool isPrimaryKey;
    std::vector<ColumnAttribute> attributes;
    DataType defaultValue;
    int length = -1;
};

class Table {
public:
    std::vector<ColumnDefinition> columns;
    std::map<std::string, size_t> columnIndex;
    std::vector<Row> rows;

    void addColumn(const ColumnDefinition &column);
    [[nodiscard]] const std::vector<ColumnDefinition>& getColumns() const;
    void insertRow(const std::vector<std::pair<std::string, DataType>> &row);
    [[nodiscard]] std::vector<Row> selectRows() const;
    void updateRows(const std::map<std::string, DataType> &updates);
    void deleteRows();
};