#include <fstream>
#include <string>
#include <regex>
#include <list>
#include <variant>
#include <optional>

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

class Token {
public:
    enum class Type {
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
            return std::to_string(static_cast<unsigned int>(type)) + " " + lexeme + " " + std::string{ *literal };
        } else {
            return std::to_string(static_cast<unsigned int>(type)) + " " + lexeme;
        }
    }
};

class Scanner {
public:
    Scanner(const std::string& source) : source(source) {}

    void scan_token() {
        char c = advance();
        switch (c) {
            case '(': addToken(Token::Type::LPAREN   , "("); break;
            case ')': addToken(Token::Type::RPAREN   , ")"); break;
            case '{': addToken(Token::Type::LBRACE   , "{"); break;
            case '}': addToken(Token::Type::RBRACE   , "}"); break;
            case ',': addToken(Token::Type::COMMA    , ","); break;
            case '.': addToken(Token::Type::DOT      , "."); break;
            case '-': addToken(Token::Type::MINUS    , "-"); break;
            case '+': addToken(Token::Type::PLUS     , "+"); break;
            case ';': addToken(Token::Type::SEMICOLON, ";"); break;
            case '*': addToken(Token::Type::ASTERISK , "*"); break;
            case '!': {
                match('=')
                    ? addToken(Token::Type::NOTEQ, "!=")
                    : addToken(Token::Type::NOT, "!");
            } break;
            case '=': {
                match('=')
                    ? addToken(Token::Type::EQEQ, "==")
                    : addToken(Token::Type::EQ, "=");
            } break;
            case '<': {
                match('=')
                    ? addToken(Token::Type::LTEQ, "<=")
                    : addToken(Token::Type::LT, "<");
            } break;
            case '>': {
                match('=')
                    ? addToken(Token::Type::GTEQ, ">=")
                    : addToken(Token::Type::GT, ">");
            } break;
            case '/': {
                if (match('/')) {
                    while (peek() != '\n' && !at_end()) advance();
                } else {
                    addToken(Token::Type::SLASH, "/");
                }
            } break;
            case ' ' :
            case '\r':
            case '\t':
            case '\n':  // line tracking handled in advance();
                break;  // Ignore whitespace

            default:
                error(line, "Unexpected character.");
                break;
        }
    }

    char peek() {
        if (at_end()) return '\0';
        return source[current];
    }

    char peekNext() {
        if (current + 1 >= source.size()) return '\0';
        return source[current + 1];
    }

    bool match(char expected) {
        if (at_end()) return false;
        if (source[current] != expected) return false;

        current++;
        return true;
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool isAlpha(char c) {
        return (c >= 'a' && c <= 'z') ||
               (c >= 'A' && c <= 'Z');
    }

    bool isAlphanumeric(char c) {
        return isAlpha(c) || isDigit(c);
    }

    void error(size_t line, const std::string& message) {
        had_error = true;
        std::cerr << "[line " << line << "] Error: " << message << std::endl;
    }

    std::list<Token> scan_tokens() {
        while (!at_end()) {
            start = current;
            scan_token();
        }

        tokens.push_back(Token(Token::Type::EndOfFile, "", line));
        return tokens;
    }

    void addToken(Token::Type type) {
        addToken(type, "");
    }

    void addToken(Token::Type type, std::string lexeme) {
        tokens.push_back(Token {type, lexeme, line});
    }

    void addToken(Token::Type type, std::string lexeme, Literal literal) {
        tokens.push_back(Token {type, lexeme, literal, line});
    }

    char advance() {
        if (at_end()) return '\0';
        char c = source[current++];
        linepos++;
        if (c == '\n') {
            linepos = 0;
            line++;
        }
        return c;
    }

    bool at_end() { return current >= source.size(); }

private:
    bool had_error = false;
    std::string source;
    size_t start = 0;
    size_t current = 0;
    size_t line = 1;
    size_t linepos = 0;
    std::list<Token> tokens;
};