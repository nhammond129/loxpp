#pragma once

#include <optional>
#include <string>
#include <variant>


class Token {
public:
    class Identifier {
    public:
        constexpr Identifier(std::string name) : name(name) {}
        std::string name;
    };

    struct Literal {
        std::variant<Identifier, std::string, double> value;
        Literal(Identifier value) : value(value) {}
        Literal(std::string value) : value(value) {}
        Literal(double value) : value(value) {}

        operator std::string() const {
            return std::visit([](auto&& arg) -> std::string {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, Identifier>) {
                    return arg.name;
                } else if constexpr (std::is_same_v<T, std::string>) {
                    return arg;
                } else if constexpr (std::is_same_v<T, double>) {
                    return std::to_string(arg);
                }
            }, value);
        }
    };

    struct Type {
        enum class type_t {
            // Single-char tokens
            LPAREN, RPAREN, LBRACE, RBRACE,
            COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, ASTERISK,

            // One or two char tokens
            NOT, NOTEQ,
            EQ , EQEQ,
            GT , GTEQ,
            LT , LTEQ,

            // Literals
            IDENTIFIER, STRING, NUMBER,

            // Keywords
            AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
            PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

            // EOF
            EndOfFile
        } type;
        using enum type_t;

        constexpr Type() : type(EndOfFile) {}
        constexpr Type(type_t type) : type(type) {}
        operator std::string() const {
            switch (type) {
                case LPAREN    : return "LPAREN";
                case RPAREN    : return "RPAREN";
                case LBRACE    : return "LBRACE";
                case RBRACE    : return "RBRACE";
                case COMMA     : return "COMMA";
                case DOT       : return "DOT";
                case MINUS     : return "MINUS";
                case PLUS      : return "PLUS";
                case SEMICOLON : return "SEMICOLON";
                case SLASH     : return "SLASH";
                case ASTERISK  : return "ASTERISK";
                case NOT       : return "NOT";
                case NOTEQ     : return "NOTEQ";
                case EQ        : return "EQ";
                case EQEQ      : return "EQEQ";
                case GT        : return "GT";
                case GTEQ      : return "GTEQ";
                case LT        : return "LT";
                case LTEQ      : return "LTEQ";
                case IDENTIFIER: return "IDENTIFIER";
                case STRING    : return "STRING";
                case NUMBER    : return "NUMBER";
                case AND       : return "AND";
                case CLASS     : return "CLASS";
                case ELSE      : return "ELSE";
                case FALSE     : return "FALSE";
                case FUN       : return "FUN";
                case FOR       : return "FOR";
                case IF        : return "IF";
                case NIL       : return "NIL";
                case OR        : return "OR";
                case PRINT     : return "PRINT";
                case RETURN    : return "RETURN";
                case SUPER     : return "SUPER";
                case THIS      : return "THIS";
                case TRUE      : return "TRUE";
                case VAR       : return "VAR";
                case WHILE     : return "WHILE";
                case EndOfFile : return "EOF";
                default:
                    return "Unknown";
            }
        }
        bool operator==(const Type& other) const {
            return type == other.type;
        }
    };

    const Type type;
    const std::string lexeme;
    const std::optional<Literal> literal;
    const size_t line_no;

    Token(Type type, std::string lexeme, std::optional<Literal> literal, size_t line_no)
        : type(type), lexeme(lexeme), literal(literal), line_no(line_no) {}
    Token(Type type, std::string lexeme, size_t line_no)
        : type(type), lexeme(lexeme), literal(std::nullopt), line_no(line_no) {}

    operator std::string() const {
        if (literal.has_value()) {
            return std::string{ type } + " " + lexeme + " " + std::string{ *literal };
        } else {
            return std::string{ type } + " " + lexeme;
        }
    }
};