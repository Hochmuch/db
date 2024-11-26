#pragma once
#include "lexer.h"
#include "../storage/database.h"

class Parser {
public:
    Parser(Lexer &lexer);
    void parse(Database &db);

private:
    Lexer &lexer;
    std::vector<Token> tokens;
    size_t current;

    bool isAtEnd() const;
    Token advance();
    Token previous() const;
    bool match(TokenType type);
    bool check(TokenType type) const;
    void createTable(Database &db);
    ColumnDefinition parseColumnDefinition();
};