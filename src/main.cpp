#include <iostream>
#include "parser.hpp"

namespace TestCases {
    constexpr std::string source = "(){}.,+;-*";
    const Token expected[] = {
        Token { Token::Type::LPAREN   , "(", 1 },
        Token { Token::Type::RPAREN   , ")", 1 },
        Token { Token::Type::LBRACE   , "{", 1 },
        Token { Token::Type::RBRACE   , "}", 1 },
        Token { Token::Type::DOT      , ".", 1 },
        Token { Token::Type::COMMA    , ",", 1 },
        Token { Token::Type::PLUS     , "+", 1 },
        Token { Token::Type::SEMICOLON, ";", 1 },
        Token { Token::Type::ASTERISK , "*", 1 },
        Token { Token::Type::MINUS    , "-", 1 },
        Token { Token::Type::EndOfFile, "",  1 }
    };
}

int main() {
    std::cout << "Hello, World!" << std::endl;
}