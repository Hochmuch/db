#pragma once
#include <string>
#include <vector>

enum class TokenType {
    IDENTIFIER,
    KEYWORD,
    SYMBOL,
    ATTRIBUTE,
    DATATYPE,
    NUMBER,
    DEFAULT_VALUE,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string text;
};

class Lexer {
public:
    explicit Lexer(const std::string &input);

    std::vector<Token> tokenize();

private:
    std::string input;
    size_t current;

    char advance();

    [[nodiscard]] char peek() const;

    void skipWhitespace();

    Token symbol();

    Token attribute();

    Token number();

    Token scanToken();
};
