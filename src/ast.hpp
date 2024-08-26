#pragma once

#include <variant>

#include "token.hpp"

class Expr {}; // class Expr

class BinaryOp : public Expr {
public:
    BinaryOp(Expr left, Token op, Expr right) : left(left), op(op), right(right) {}

private:
    Expr left;
    Token op;
    Expr right;
}; // class BinaryOp

class UnaryOp : public Expr {
public:
    UnaryOp(Token op, Expr right) : op(op), right(right) {}

private:
    Token op;
    Expr right;
}; // class UnaryOp

class Grouping : public Expr {
public:
    Grouping(Expr expr) : subexpr(expr) {}

private:
    Expr subexpr;
}; // class Grouping

class Literal : public Expr {
public:
    Literal(Token::Literal value) : value(value) {}
    Literal() : value(Token::Literal()) {}

private:
    Token::Literal value;
}; // class Literal