#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace qvault {

enum class TokenType {
    // Keywords
    Select,
    Where,
    And,
    Or,
    Not,
    True,
    False,
    Null,

    // Identifiers & Literals
    Identifier,
    String,
    Number,

    // Operators & Punctuation
    Equal,            // ==
    NotEqual,         // !=
    GreaterThan,      // >
    LessThan,         // <
    GreaterOrEqual,   // >=
    LessOrEqual,      // <=
    Comma,            // ,
    LeftParen,        // (
    RightParen,       // )

    // Special
    EndOfFile,
    Invalid
};

std::string to_string(TokenType type);

struct Token {
    TokenType type;
    std::string lexeme;
    size_t line;
    size_t column;

    Token(TokenType t, std::string lex, size_t l, size_t col)
        : type(t), lexeme(std::move(lex)), line(l), column(col) {}

    std::string to_string() const;
};

class LexerException : public std::runtime_error {
public:
    LexerException(const std::string& message, size_t line, size_t column)
        : std::runtime_error(message), line_(line), column_(column) {}

    size_t line() const { return line_; }
    size_t column() const { return column_; }

private:
    size_t line_;
    size_t column_;
};

class QueryLexer {
public:
    explicit QueryLexer(std::string source);

    std::vector<Token> tokenize();

private:
    char peek() const;
    char peek_next() const;
    char advance();
    bool is_at_end() const;
    bool match(char expected);

    void skip_whitespace();
    Token scan_token();
    Token scan_string(char quote_char);
    Token scan_number();
    Token scan_identifier_or_keyword();

    std::string source_;
    size_t start_ = 0;
    size_t current_ = 0;
    size_t line_ = 1;
    size_t column_ = 1;
    size_t start_column_ = 1;
};

} // namespace qvault
