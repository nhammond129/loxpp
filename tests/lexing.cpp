#include <gtest/gtest.h>

#include <parser.hpp>


namespace SingleCharTokensTest {
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
        Token { Token::Type::MINUS    , "-", 1 },
        Token { Token::Type::ASTERISK , "*", 1 },
        Token { Token::Type::EndOfFile, "" , 1 }
    };
}

TEST(Scanner, SingleCharTokens) {
    Scanner scanner(SingleCharTokensTest::source);
    auto tokens = scanner.scan_tokens();

    auto expected = SingleCharTokensTest::expected;
    for (auto& token : tokens) {
        EXPECT_EQ((int)token.type, (int)expected->type);
        EXPECT_EQ(token.lexeme, expected->lexeme);
        EXPECT_EQ(token.line_no, expected->line_no);
        expected++;
    }
}