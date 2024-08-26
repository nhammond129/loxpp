#include <gtest/gtest.h>

#include <lexer.hpp>


namespace SingleCharTokensTest {
    const std::string source = "(){}.,+;-*";
    const Token expected[] = {
        Token { Token::Type::LPAREN   , "(", 1 },
        Token { Token::Type::RPAREN   , ")", 1 },
        Token { Token::Type::LBRACE   , "{", 1 },
        Token { Token::Type::RBRACE   , "}", 1 },
        Token { Token::Type::DOT      , ".", 1 },
        Token { Token::Type::COMMA    , ",", 1 },
        Token { Token::Type::PLUS     , "+", 1 },
        Token { Token::Type::SEMICOLON, ";", 1 },
        Token { Token::Type::MINUS    , "-", 1 },
        Token { Token::Type::ASTERISK , "*", 1 },
        Token { Token::Type::EndOfFile, "" , 1 }
    };
}

TEST(Lexer, SingleCharTokens) {
    Lexer scanner(SingleCharTokensTest::source);
    auto tokens = scanner.scan_tokens();

    auto expected = SingleCharTokensTest::expected;
    for (auto& token : tokens) {
        std::cout << std::string{ token } << std::endl;
        EXPECT_EQ(std::string{ token.type }, std::string{ expected->type });
        EXPECT_EQ(token.lexeme, expected->lexeme);
        EXPECT_EQ(token.line_no, expected->line_no);
        expected++;
    }
}

namespace HelloWorld {
    const std::string source =
        "// Your first Lox program!\n"
        "print \"Hello, world!\";\n"
        "1 + 2 * 3 / 4 - 5;";
    const Token expected[] = {
        Token { Token::Type::PRINT    , "print", 2 },
        Token { Token::Type::STRING   , "Hello, world!", 2 },
        Token { Token::Type::SEMICOLON, ";", 2 },
        Token { Token::Type::NUMBER   , "1", 3 },
        Token { Token::Type::PLUS     , "+", 3 },
        Token { Token::Type::NUMBER   , "2", 3 },
        Token { Token::Type::ASTERISK , "*", 3 },
        Token { Token::Type::NUMBER   , "3", 3 },
        Token { Token::Type::SLASH    , "/", 3 },
        Token { Token::Type::NUMBER   , "4", 3 },
        Token { Token::Type::MINUS    , "-", 3 },
        Token { Token::Type::NUMBER   , "5", 3 },
        Token { Token::Type::SEMICOLON, ";", 3 },
        Token { Token::Type::EndOfFile, "" , 3 }
    };
}

TEST(Lexer, HelloWorld) {
    Lexer scanner(HelloWorld::source);
    auto tokens = scanner.scan_tokens();

    auto expected = HelloWorld::expected;
    for (auto& token : tokens) {
        std::cout << std::string{ token } << std::endl;
        EXPECT_EQ(std::string{ token.type }, std::string{ expected->type });
        EXPECT_EQ(token.lexeme, expected->lexeme);
        EXPECT_EQ(token.line_no, expected->line_no);
        expected++;
    }
}