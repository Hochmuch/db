#include <iostream>
#include <vector>

#include "modules/language/lexer.h"
#include "modules/language/parser.h"
#include "modules/storage/database.h"
#include "modules/storage/table.h"

int main() {
    std::string createTableQuery =
            "create TABLE users ({key, autoincrement} id : int32, {unique} login: string[10], password_hash: bytes[2], is_admin: bool = false)";

    std::vector<std::vector<std::pair<std::string, DataType> > > rowsToInsert = {
        {
            {"id", 1}, {"login", "user1"}, {"password_hash", std::vector<std::byte>{std::byte('a'), std::byte('1')}},
            {"is_admin", false}
        },
        {
            {"id", 2}, {"login", "user2"}, {"password_hash", std::vector<std::byte>{std::byte('b'), std::byte('2')}},
            {"is_admin", true}
        },
        {
            {"id", 3}, {"login", "user3"}, {"password_hash", std::vector<std::byte>{std::byte('c'), std::byte('3')}},
            {"is_admin", false}
        }
    };
    // если заменить, например, имя любого юзера на слишком длинное(>10), можно увидеть, что ошибка корректно обрабатывается

    Lexer lexer(createTableQuery);

    Database db;
    Parser parser(lexer);

    try {
        parser.parse(db);
        std::cout << "Table created successfully." << std::endl;

        for (const auto &row: rowsToInsert) {
            db.insertIntoTable("users", row);
        }
        std::cout << "Rows inserted successfully." << std::endl;

        std::vector<Row> rows = db.selectFromTable("users");
        std::cout << "Rows in Table 'users':" << std::endl;

        const auto &columns = db.getTables().at("users").getColumns();
        for (const auto &column: columns) {
            std::cout << column.name << "\t";
        }
        std::cout << std::endl;

        for (const auto &row: rows) {
            for (const auto &col: row) {
                std::visit([](auto &&arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, int32_t>)
                        std::cout << arg << "\t";
                    else if constexpr (std::is_same_v<T, bool>)
                        std::cout << (arg ? "true" : "false") << "\t";
                    else if constexpr (std::is_same_v<T, std::string>)
                        std::cout << arg << "\t";
                    else if constexpr (std::is_same_v<T, std::vector<std::byte> >) {
                        for (const auto &b: arg)
                            std::cout << static_cast<char>(b);
                        std::cout << "\t";
                    }
                }, col);
            }
            std::cout << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
