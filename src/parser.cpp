#include "parser.hpp"

Expr Parser::parse() {
    return expression();
}

// expression -> equality
Expr Parser::expression() {
    return equality();
}

// equality -> comparison ( ( "!=" | "==" ) comparison )*
Expr Parser::equality() {
    Expr expr = comparison();

    while (match({ Token::Type::NOTEQ, Token::Type::EQEQ })) {
        Token op = previous();
        Expr right = comparison();
        expr = BinaryOp { expr, op, right };
    }

    return expr;
}

// comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
Expr Parser::comparison() {
    Expr expr = term();

    while (match({ Token::Type::GT, Token::Type::GTEQ, Token::Type::LT, Token::Type::LTEQ })) {
        Token op = previous();
        Expr right = term();
        expr = BinaryOp { expr, op, right };
    }

    return expr;
}

// term -> unary ( ( "-" | "+" ) unary )*
Expr Parser::term() {
    Expr expr = unary();

    while (match({ Token::Type::MINUS, Token::Type::PLUS })) {
        Token op = previous();
        Expr right = unary();
        expr = BinaryOp { expr, op, right };
    }

    return expr;
}

// unary -> ( "!" | "-" ) unary | primary
Expr Parser::unary() {
    if (match({ Token::Type::NOT, Token::Type::MINUS })) {
        Token op = previous();
        Expr right = unary();
        return UnaryOp { op, right };
    }

    return primary();
}

// primary -> NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")"
Expr Parser::primary() {
    if (match({ Token::Type::TRUE  })) { return Literal { true  }; }
    if (match({ Token::Type::FALSE })) { return Literal { false }; }
    if (match({ Token::Type::NIL   })) { return Literal {       }; }

    if (match({ Token::Type::NUMBER, Token::Type::STRING })) {
        return Literal { previous().literal.value() };
    }

    if (match({ Token::Type::LPAREN })) {
        Expr expr = expression();
        if (!match({ Token::Type::RPAREN })) {
            error(previous(), "Expect ')' after expression.");
        }
        return Grouping { expr };
    }

    error(peek(), "Expect expression.");
}