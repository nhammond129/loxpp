#include <string>
#include <list>
#include <iostream>

#include "token.hpp"

class Lexer {
public:
    Lexer(const std::string& source) : source(source) {}

    std::list<Token> scan_tokens();

    void scan_token();

    char advance();
    char peek() const;
    char peekNext() const;

    bool match(char expected);
    bool at_end() const;

    static bool isDigit(char c);
    static bool isAlpha(char c);
    static bool isAlphanum(char c);

    void addToken(Token::Type type);
    void addToken(Token::Type type, std::string lexeme);
    void addToken(Token::Type type, std::string lexeme, Token::Literal literal);

    void error(size_t line, const std::string& message);

private:
    bool had_error = false;
    std::string source;
    size_t start = 0;
    size_t current = 0;
    size_t line = 1;
    size_t linepos = 0;
    std::list<Token> tokens;
};