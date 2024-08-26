#include "lexer.hpp"

std::list<Token> Lexer::scan_tokens() {
    while (!at_end()) {
        start = current;
        scan_token();
    }

    addToken(Token::Type::EndOfFile);
    return tokens;
}

void Lexer::scan_token() {
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
            } else if (match('*')) {
                size_t nesting = 1;
                while (nesting > 0 && !at_end()) {
                    if (peek() == '/' && peekNext() == '*') {
                        nesting++;
                        advance();  // Consume '/'
                    } else if (peek() == '*' && peekNext() == '/') {
                        nesting--;
                        advance();  // Consume '*'
                    }
                    advance();  // Consume second-half of comment-delim or intermediate char
                }

                if (at_end() && nesting > 0) {
                    error(line, "Unterminated block comment.");
                    return;
                }
            } else {
                addToken(Token::Type::SLASH, "/");
            }
        } break;
        case ' ' :
        case '\r':
        case '\t':
        case '\n':  // line tracking handled in advance();
            break;  // Ignore whitespace
        case '"': { // String literals
            while (peek() != '"' && !at_end()) {
                if (peek() == '\n') line++;
                advance();
            }

            if (at_end()) {
                error(line, "Unterminated string.");
                return;
            }

            advance();  // Consume closing "
            std::string value = source.substr(start + 1, current - start - 2);
            addToken(Token::Type::STRING, value, Literal(value));
        } break;

        default:
            if (isDigit(c)) {
                while (isDigit(peek())) advance();

                if (peek() == '.' && isDigit(peekNext())) {
                    advance();  // Consume '.'
                    while (isDigit(peek())) advance();
                }

                addToken(Token::Type::NUMBER, source.substr(start, current - start), Literal(std::stod(source.substr(start, current - start))));
            } else if (isAlpha(c)) {
                while (isAlphanum(peek()) || peek() == '_') advance();

                std::string text = source.substr(start, current - start);
                Token::Type type;
                if      (text == "and"   ) type = Token::Type::AND;
                else if (text == "class" ) type = Token::Type::CLASS;
                else if (text == "else"  ) type = Token::Type::ELSE;
                else if (text == "false" ) type = Token::Type::FALSE;
                else if (text == "for"   ) type = Token::Type::FOR;
                else if (text == "fun"   ) type = Token::Type::FUN;
                else if (text == "if"    ) type = Token::Type::IF;
                else if (text == "nil"   ) type = Token::Type::NIL;
                else if (text == "or"    ) type = Token::Type::OR;
                else if (text == "print" ) type = Token::Type::PRINT;
                else if (text == "return") type = Token::Type::RETURN;
                else if (text == "super" ) type = Token::Type::SUPER;
                else if (text == "this"  ) type = Token::Type::THIS;
                else if (text == "true"  ) type = Token::Type::TRUE;
                else if (text == "var"   ) type = Token::Type::VAR;
                else if (text == "while" ) type = Token::Type::WHILE;
                else {
                    type = Token::Type::IDENTIFIER;
                    addToken(type, text, Literal(Identifier(text)));
                    break;
                }
                addToken(type, text);
            } else {
                error(line, "Unexpected character.");
                break;
            }
    }
}

char Lexer::peek() const {
    if (at_end()) return '\0';
    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= source.size()) return '\0';
    return source[current + 1];
}

char Lexer::advance() {
    if (at_end()) return '\0';
    char c = source[current++];
    linepos++;
    if (c == '\n') {
        linepos = 0;
        line++;
    }
    return c;
}

bool Lexer::match(char expected) {
    if (at_end()) return false;
    if (peek() != expected) return false;

    current++;
    return true;
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z');
}

bool Lexer::isAlphanum(char c) {
    return (isAlpha(c) || isDigit(c));
}

bool Lexer::at_end() const {
    return current >= source.size();
}

void Lexer::error(size_t line, const std::string& message) {
    had_error = true;
    std::cerr << "[line " << line << "] Lexing Error: " << message << std::endl;
}

void Lexer::addToken(Token::Type type, std::string lexeme, Literal literal) {
    tokens.push_back(Token {type, lexeme, literal, line});
}

void Lexer::addToken(Token::Type type, std::string lexeme) {
    tokens.push_back(Token {type, lexeme, line});
}

void Lexer::addToken(Token::Type type) {
    addToken(type, "");
}