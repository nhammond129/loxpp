#pragma once

#include <variant>

#include "tokens.hpp"

class Expr {
    Expr(BinaryOp expr) : expr(expr) {}
    Expr(UnaryOp  expr) : expr(expr) {}
    Expr(Literal  expr) : expr(expr) {}
    Expr(Grouping expr) : expr(expr) {}

    std::variant<BinaryOp, UnaryOp, Literal, Grouping> expr;
}; // class Expr

class BinaryOp {
public:
    BinaryOp(Expr left, Token op, Expr right) : left(left), op(op), right(right) {}

private:
    Expr left;
    Token op;
    Expr right;
}; // class BinaryOp

class UnaryOp {
public:
    UnaryOp(Token op, Expr right) : op(op), right(right) {}

private:
    Token op;
    Expr right;
}; // class UnaryOp

class Grouping {
public:
    Grouping(Expr expr) : expr(expr) {}

private:
    Expr expr;
}; // class Grouping