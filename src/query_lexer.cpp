#include "query_lexer.hpp"
#include <cctype>
#include <algorithm>
#include <sstream>

namespace qvault {

std::string to_string(TokenType type) {
    switch (type) {
        case TokenType::Select:         return "SELECT";
        case TokenType::Where:          return "WHERE";
        case TokenType::And:            return "AND";
        case TokenType::Or:             return "OR";
        case TokenType::Not:            return "NOT";
        case TokenType::True:           return "TRUE";
        case TokenType::False:          return "FALSE";
        case TokenType::Null:           return "NULL";
        case TokenType::Identifier:     return "IDENTIFIER";
        case TokenType::String:         return "STRING";
        case TokenType::Number:         return "NUMBER";
        case TokenType::Equal:          return "EQUAL";
        case TokenType::NotEqual:       return "NOT_EQUAL";
        case TokenType::GreaterThan:    return "GREATER_THAN";
        case TokenType::LessThan:       return "LESS_THAN";
        case TokenType::GreaterOrEqual: return "GREATER_OR_EQUAL";
        case TokenType::LessOrEqual:    return "LESS_OR_EQUAL";
        case TokenType::Comma:          return "COMMA";
        case TokenType::LeftParen:      return "LEFT_PAREN";
        case TokenType::RightParen:     return "RIGHT_PAREN";
        case TokenType::EndOfFile:      return "EOF";
        case TokenType::Invalid:        return "INVALID";
    }
    return "UNKNOWN";
}

std::string Token::to_string() const {
    std::ostringstream oss;
    oss << "[" << qvault::to_string(type) << " \"" << lexeme << "\" (" << line << ":" << column << ")]";
    return oss.str();
}

QueryLexer::QueryLexer(std::string source)
    : source_(std::move(source)), start_(0), current_(0), line_(1), column_(1), start_column_(1) {}

char QueryLexer::peek() const {
    if (is_at_end()) return '\0';
    return source_[current_];
}

char QueryLexer::peek_next() const {
    if (current_ + 1 >= source_.length()) return '\0';
    return source_[current_ + 1];
}

char QueryLexer::advance() {
    char c = source_[current_++];
    column_++;
    return c;
}

bool QueryLexer::is_at_end() const {
    return current_ >= source_.length();
}

bool QueryLexer::match(char expected) {
    if (is_at_end()) return false;
    if (source_[current_] != expected) return false;
    current_++;
    column_++;
    return true;
}

void QueryLexer::skip_whitespace() {
    while (!is_at_end()) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\t':
                advance();
                break;
            case '\r':
                advance();
                break;
            case '\n':
                advance();
                line_++;
                column_ = 1;
                break;
            case '-':
                // Single-line comment: -- comment
                if (peek_next() == '-') {
                    while (!is_at_end() && peek() != '\n') {
                        advance();
                    }
                } else {
                    return;
                }
                break;
            case '/':
                // Single-line comment: // comment
                if (peek_next() == '/') {
                    while (!is_at_end() && peek() != '\n') {
                        advance();
                    }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

Token QueryLexer::scan_token() {
    start_ = current_;
    start_column_ = column_;

    char c = advance();

    // Check for negative number
    if (c == '-' && std::isdigit(static_cast<unsigned char>(peek()))) {
        return scan_number();
    }

    switch (c) {
        case ',':
            return Token(TokenType::Comma, ",", line_, start_column_);
        case '(':
            return Token(TokenType::LeftParen, "(", line_, start_column_);
        case ')':
            return Token(TokenType::RightParen, ")", line_, start_column_);
        case '>':
            if (match('=')) {
                return Token(TokenType::GreaterOrEqual, ">=", line_, start_column_);
            }
            return Token(TokenType::GreaterThan, ">", line_, start_column_);
        case '<':
            if (match('=')) {
                return Token(TokenType::LessOrEqual, "<=", line_, start_column_);
            }
            return Token(TokenType::LessThan, "<", line_, start_column_);
        case '=':
            if (match('=')) {
                return Token(TokenType::Equal, "==", line_, start_column_);
            }
            throw LexerException("Unexpected single '='. In QQL, equality comparison is '=='.", line_, start_column_);
        case '!':
            if (match('=')) {
                return Token(TokenType::NotEqual, "!=", line_, start_column_);
            }
            throw LexerException("Unexpected '!'. Did you mean '!='?", line_, start_column_);
        case '"':
        case '\'':
            return scan_string(c);
        default:
            if (std::isdigit(static_cast<unsigned char>(c))) {
                return scan_number();
            }
            if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                return scan_identifier_or_keyword();
            }
            throw LexerException(std::string("Unexpected character: '") + c + "'", line_, start_column_);
    }
}

Token QueryLexer::scan_string(char quote_char) {
    std::string value;

    while (!is_at_end() && peek() != quote_char) {
        if (peek() == '\n') {
            line_++;
            column_ = 1;
        }

        if (peek() == '\\') {
            advance(); // consume '\'
            if (is_at_end()) {
                throw LexerException("Unterminated escape sequence in string literal", line_, start_column_);
            }
            char esc = advance();
            switch (esc) {
                case '"':  value += '"'; break;
                case '\'': value += '\''; break;
                case '\\': value += '\\'; break;
                case 'n':  value += '\n'; break;
                case 't':  value += '\t'; break;
                case 'r':  value += '\r'; break;
                default:   value += esc;  break;
            }
        } else {
            value += advance();
        }
    }

    if (is_at_end()) {
        throw LexerException("Unterminated string literal", line_, start_column_);
    }

    advance(); // consume closing quote
    return Token(TokenType::String, value, line_, start_column_);
}

Token QueryLexer::scan_number() {
    while (std::isdigit(static_cast<unsigned char>(peek()))) {
        advance();
    }

    // Look for fractional part
    if (peek() == '.' && std::isdigit(static_cast<unsigned char>(peek_next()))) {
        advance(); // consume '.'
        while (std::isdigit(static_cast<unsigned char>(peek()))) {
            advance();
        }
    }

    std::string num_str = source_.substr(start_, current_ - start_);
    return Token(TokenType::Number, num_str, line_, start_column_);
}

Token QueryLexer::scan_identifier_or_keyword() {
    while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_' || peek() == '.') {
        advance();
    }

    std::string lexeme = source_.substr(start_, current_ - start_);

    // Check against keywords (case-insensitive for SQL keywords)
    std::string upper = lexeme;
    std::transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char ch) {
        return static_cast<char>(std::toupper(ch));
    });

    if (upper == "SELECT") return Token(TokenType::Select, lexeme, line_, start_column_);
    if (upper == "WHERE")  return Token(TokenType::Where,  lexeme, line_, start_column_);
    if (upper == "AND")    return Token(TokenType::And,    lexeme, line_, start_column_);
    if (upper == "OR")     return Token(TokenType::Or,     lexeme, line_, start_column_);
    if (upper == "NOT")    return Token(TokenType::Not,    lexeme, line_, start_column_);
    if (upper == "TRUE")   return Token(TokenType::True,   lexeme, line_, start_column_);
    if (upper == "FALSE")  return Token(TokenType::False,  lexeme, line_, start_column_);
    if (upper == "NULL")   return Token(TokenType::Null,   lexeme, line_, start_column_);

    return Token(TokenType::Identifier, lexeme, line_, start_column_);
}

std::vector<Token> QueryLexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        skip_whitespace();
        if (is_at_end()) {
            break;
        }
        tokens.push_back(scan_token());
    }

    tokens.emplace_back(TokenType::EndOfFile, "", line_, column_);
    return tokens;
}

} // namespace qvault
