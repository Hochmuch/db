#include "parser.h"
#include "utils.h"
#include <sstream>

Parser::Parser(Lexer &lexer)
    : lexer(lexer), current(0) {
    tokens = lexer.tokenize();
}

void Parser::parse(Database &db) {
    while (!isAtEnd()) {
        if (match(TokenType::KEYWORD) && previous().text == "CREATE") {
            createTable(db);
        } else {
            advance();
        }
    }
}

bool Parser::isAtEnd() const {
    return current >= tokens.size() || tokens[current].type == TokenType::END_OF_FILE;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::previous() const {
    if (current == 0) {
        throw std::runtime_error("No previous token, out of bounds access.");
    }
    return tokens[current - 1];
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return tokens[current].type == type;
}

void Parser::createTable(Database &db) {
    if (!match(TokenType::KEYWORD) || previous().text != "TABLE") {
        throw std::runtime_error("Expected 'TABLE' after 'CREATE' keyword.");
    }

    Token tableToken = advance();
    if (tableToken.type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected table name after 'TABLE'.");
    }

    std::string tableName = tableToken.text;
    trim(tableName);

    if (!match(TokenType::SYMBOL) || previous().text != "(") {
        throw std::runtime_error("Expected '(' after table name.");
    }

    std::vector<ColumnDefinition> columns;

    while (tokens[current].text != ")") {
        columns.push_back(parseColumnDefinition());
        if (!check(TokenType::SYMBOL) || (check(TokenType::SYMBOL) && tokens[current].text != ")")) {
            if (!match(TokenType::SYMBOL) || previous().text != ",") {
                throw std::runtime_error("Expected ',' or ')' in column definitions.");
            }
        }
    }

    if (!match(TokenType::SYMBOL) || previous().text != ")") {
        throw std::runtime_error("Expected ')' after column definitions.");
    }

    db.createTable(tableName, columns);
}

ColumnDefinition Parser::parseColumnDefinition() {
    std::vector<ColumnAttribute> attributes;
    bool isPrimaryKey = false;

    if (check(TokenType::ATTRIBUTE)) {
        Token attributeToken = advance();
        std::stringstream attributeStream(attributeToken.text);
        std::string token;

        while (std::getline(attributeStream, token, ',')) {
            trim(token);
            if (token == "key") isPrimaryKey = true;
            if (token == "autoincrement") attributes.push_back(ColumnAttribute::AUTOINCREMENT);
            if (token == "unique") attributes.push_back(ColumnAttribute::UNIQUE);
        }
    }

    Token columnToken = advance();
    if (columnToken.type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected column name.");
    }

    std::string columnName = columnToken.text;
    trim(columnName);

    if (!match(TokenType::SYMBOL) || previous().text != ":") {
        throw std::runtime_error("Expected ':' after column name.");
    }

    Token typeToken = advance();
    if (typeToken.type != TokenType::DATATYPE) {
        throw std::runtime_error("Expected data type after ':'.");
    }

    std::string columnType = typeToken.text;
    trim(columnType);

    int length = -1;

    if (typeToken.text == "string" || typeToken.text == "bytes") {
        Token lenToken = advance();
        if (lenToken.text != "[") {
            throw std::runtime_error("Expected '['.");
        }

        lenToken = advance();
        if (lenToken.type != TokenType::NUMBER) {
            throw std::runtime_error("Expected length.");
        }

        length = std::stoi(lenToken.text);

        lenToken = advance();
        if (lenToken.text != "]") {
            throw std::runtime_error("Expected ']'.");
        }
    }


    DataType defaultValue;
    if (check(TokenType::SYMBOL) && tokens[current].text == "=") {
        advance();
        Token defaultToken = advance();
        if (columnType == "int32") {
            defaultValue = std::stoi(defaultToken.text);
        } else if (columnType == "bool") {
            defaultValue = defaultToken.text == "true";
        } else if (columnType.find("string") != std::string::npos) {
            defaultValue = defaultToken.text;
        } else if (columnType.find("bytes") != std::string::npos) {
            std::vector<std::byte> byteVector;
            for (char c : defaultToken.text) {
                byteVector.push_back(static_cast<std::byte>(c));
            }
            defaultValue = byteVector;
        }
    }

    return {columnName, columnType, isPrimaryKey, attributes, defaultValue, length};
}