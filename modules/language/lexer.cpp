#include "lexer.h"
#include <unordered_map>
#include <cctype>
#include <iostream>
#include <set>

#include "utils.h"

Lexer::Lexer(const std::string &input) : input(input), current(0) {
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (current < input.size()) {
        skipWhitespace();
        if (current < input.size()) {
            tokens.push_back(scanToken());
        }
    }
    tokens.push_back(Token{TokenType::END_OF_FILE, ""});
    return tokens;
}

char Lexer::advance() {
    return input[current++];
}

char Lexer::peek() const {
    if (current < input.size()) {
        return input[current];
    }
    return '\0';
}

void Lexer::skipWhitespace() {
    while (current < input.size() && std::isspace(static_cast<unsigned char>(input[current]))) {
        ++current;
    }
}

Token Lexer::symbol() {
    char c = advance();
    return Token{TokenType::SYMBOL, std::string(1, c)};
}

Token Lexer::attribute() {
    size_t start = current;
    while (current < input.size() && input[current] != '}') {
        advance();
    }
    advance();
    std::string text = input.substr(start, current - start - 1);
    return Token{TokenType::ATTRIBUTE, text};
}

Token Lexer::number() {
    size_t start = current;
    while (current < input.size() && std::isdigit(static_cast<unsigned char>(input[current]))) {
        advance();
    }
    std::string text = input.substr(start, current - start);
    return Token{TokenType::NUMBER, text};
}

//

Token Lexer::scanToken() {
    char c = peek();

    if (std::isalpha(static_cast<unsigned char>(c))) {
        size_t start = current;
        while (current < input.size() && (std::isalnum(static_cast<unsigned char>(input[current])) || input[current] ==
                                          '_')) {
            advance();
        }
        std::string text = input.substr(start, current - start);

        static const std::set<std::string> keywords = {
            "CREATE", "TABLE"
        };
        if (keywords.contains(toUpper(text))) {
            return {TokenType::KEYWORD, toUpper(text)};
        }

        static const std::set<std::string> dataTypes = {
            "int32", "bool", "string", "bytes"
        };

        if (dataTypes.contains(text)) {
            return Token{TokenType::DATATYPE, text};
        }

        return Token{TokenType::IDENTIFIER, text};
    }
    if (c == '(' || c == ')' || c == ',' || c == ':' || c == '=' || c == '[' || c == ']') {
        return symbol();
    }
    if (c == '{') {
        advance();
        return attribute();
    }
    if (std::isdigit(c)) {
        return number();
    }
    advance();
    return Token{TokenType::END_OF_FILE, ""};
}
