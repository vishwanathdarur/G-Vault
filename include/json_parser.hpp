#pragma once

#include "common.hpp"
#include <string>
#include <stdexcept>

namespace gvault {

class ParseException : public std::runtime_error {
public:
    ParseException(const std::string& message, size_t line = 0, size_t column = 0)
        : std::runtime_error(message), line_(line), column_(column) {}
    
    size_t line() const { return line_; }
    size_t column() const { return column_; }

private:
    size_t line_;
    size_t column_;
};

class JsonParser {
public:
    JsonParser();
    
    // Parse JSON string
    JsonValue parse(const std::string& json_string);
    
    // Parse JSON from file
    JsonValue parse_file(const std::string& filename);

private:
    // Tokenizer
    enum class TokenType {
        EndOfFile,
        LeftBrace,      // {
        RightBrace,     // }
        LeftBracket,    // [
        RightBracket,   // ]
        Colon,          // :
        Comma,          // ,
        String,
        Number,
        True,
        False,
        Null
    };

    struct Token {
        TokenType type;
        std::string value;
        size_t line;
        size_t column;
    };

    // Lexer
    std::vector<Token> tokenize(const std::string& json_string);
    Token next_token();
    void skip_whitespace();
    Token read_string();
    Token read_number();
    
    // Parser
    JsonValue parse_value();
    JsonValue parse_object();
    JsonValue parse_array();
    JsonValue parse_string();
    JsonValue parse_number();

    // Helpers
    void consume(TokenType expected);
    Token peek() const;
    Token advance();
    bool match(TokenType type) const;

    // Member variables
    std::string input_;
    size_t position_;
    size_t line_;
    size_t column_;
    std::vector<Token> tokens_;
    size_t token_index_;
};

} // namespace gvault
