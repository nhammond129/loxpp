#pragma once

#include <list>

#include "token.hpp"
#include "ast.hpp"

class Parser {
public:
    Parser(const std::list<Token>& tokens) : tokens(tokens) {}

    Expr parse();

private:
    Expr expression();
    Expr equality();
    Expr comparison();
    Expr term();
    Expr unary();
    Expr primary();

    bool match(const std::list<Token::Token::Type>& types);
    bool check(const Token::Token::Type& type);

    void advance();
    Token peek();
    Token peekNext();
    Token previous();

    bool at_end() const;

    void error(const Token& token, const std::string& message);

    std::list<Token> tokens;
    std::list<Token>::const_iterator current = tokens.begin();

}; // class Parser