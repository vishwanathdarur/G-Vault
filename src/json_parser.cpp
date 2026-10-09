#include "json_parser.hpp"
#include <cctype>
#include <fstream>
#include <sstream>

namespace gvault {

JsonParser::JsonParser() 
    : position_(0), line_(1), column_(1), token_index_(0) {}

JsonValue JsonParser::parse(const std::string& json_string) {
    input_ = json_string;
    position_ = 0;
    line_ = 1;
    column_ = 1;
    token_index_ = 0;
    
    tokens_ = tokenize(json_string);
    
    if (tokens_.empty()) {
        throw ParseException("Empty JSON input");
    }
    
    return parse_value();
}

JsonValue JsonParser::parse_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw ParseException("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return parse(buffer.str());
}

std::vector<JsonParser::Token> JsonParser::tokenize(const std::string& json_string) {
    std::vector<Token> tokens;
    input_ = json_string;
    position_ = 0;
    line_ = 1;
    column_ = 1;
    
    while (position_ < input_.size()) {
        skip_whitespace();
        
        if (position_ >= input_.size()) break;
        
        char c = input_[position_];
        
        if (c == '{') {
            tokens.push_back({TokenType::LeftBrace, "{", line_, column_});
            position_++;
            column_++;
        } else if (c == '}') {
            tokens.push_back({TokenType::RightBrace, "}", line_, column_});
            position_++;
            column_++;
        } else if (c == '[') {
            tokens.push_back({TokenType::LeftBracket, "[", line_, column_});
            position_++;
            column_++;
        } else if (c == ']') {
            tokens.push_back({TokenType::RightBracket, "]", line_, column_});
            position_++;
            column_++;
        } else if (c == ':') {
            tokens.push_back({TokenType::Colon, ":", line_, column_});
            position_++;
            column_++;
        } else if (c == ',') {
            tokens.push_back({TokenType::Comma, ",", line_, column_});
            position_++;
            column_++;
        } else if (c == '"') {
            tokens.push_back(read_string());
        } else if (c == '-' || std::isdigit(c)) {
            tokens.push_back(read_number());
        } else if (input_.substr(position_, 4) == "true") {
            tokens.push_back({TokenType::True, "true", line_, column_});
            position_ += 4;
            column_ += 4;
        } else if (input_.substr(position_, 5) == "false") {
            tokens.push_back({TokenType::False, "false", line_, column_});
            position_ += 5;
            column_ += 5;
        } else if (input_.substr(position_, 4) == "null") {
            tokens.push_back({TokenType::Null, "null", line_, column_});
            position_ += 4;
            column_ += 4;
        } else {
            throw ParseException("Unexpected character: " + std::string(1, c), line_, column_);
        }
    }
    
    tokens.push_back({TokenType::EndOfFile, "", line_, column_});
    return tokens;
}

void JsonParser::skip_whitespace() {
    while (position_ < input_.size() && std::isspace(input_[position_])) {
        if (input_[position_] == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
        position_++;
    }
}

JsonParser::Token JsonParser::read_string() {
    size_t start_line = line_;
    size_t start_col = column_;
    position_++; // Skip opening quote
    column_++;
    
    std::string value;
    
    while (position_ < input_.size() && input_[position_] != '"') {
        if (input_[position_] == '\\') {
            position_++;
            column_++;
            if (position_ >= input_.size()) {
                throw ParseException("Unterminated string escape", start_line, start_col);
            }
            
            char escape_char = input_[position_];
            switch (escape_char) {
                case '"':
                case '\\':
                case '/':
                    value += escape_char;
                    break;
                case 'b':
                    value += '\b';
                    break;
                case 'f':
                    value += '\f';
                    break;
                case 'n':
                    value += '\n';
                    break;
                case 'r':
                    value += '\r';
                    break;
                case 't':
                    value += '\t';
                    break;
                default:
                    throw ParseException("Invalid escape sequence: \\" + std::string(1, escape_char), 
                                        line_, column_);
            }
        } else {
            if (input_[position_] == '\n') {
                line_++;
                column_ = 1;
            } else {
                column_++;
            }
            value += input_[position_];
        }
        position_++;
    }
    
    if (position_ >= input_.size()) {
        throw ParseException("Unterminated string", start_line, start_col);
    }
    
    position_++; // Skip closing quote
    column_++;
    
    return {TokenType::String, value, start_line, start_col};
}

JsonParser::Token JsonParser::read_number() {
    size_t start = position_;
    size_t start_col = column_;
    
    if (input_[position_] == '-') {
        position_++;
        column_++;
    }
    
    while (position_ < input_.size() && std::isdigit(input_[position_])) {
        position_++;
        column_++;
    }
    
    if (position_ < input_.size() && input_[position_] == '.') {
        position_++;
        column_++;
        
        while (position_ < input_.size() && std::isdigit(input_[position_])) {
            position_++;
            column_++;
        }
    }
    
    if (position_ < input_.size() && (input_[position_] == 'e' || input_[position_] == 'E')) {
        position_++;
        column_++;
        
        if (position_ < input_.size() && (input_[position_] == '+' || input_[position_] == '-')) {
            position_++;
            column_++;
        }
        
        while (position_ < input_.size() && std::isdigit(input_[position_])) {
            position_++;
            column_++;
        }
    }
    
    std::string value = input_.substr(start, position_ - start);
    return {TokenType::Number, value, line_, start_col};
}

JsonValue JsonParser::parse_value() {
    const auto& token = peek();
    
    switch (token.type) {
        case TokenType::LeftBrace:
            return parse_object();
        case TokenType::LeftBracket:
            return parse_array();
        case TokenType::String:
            return parse_string();
        case TokenType::Number:
            return parse_number();
        case TokenType::True:
            advance();
            return JsonValue(true);
        case TokenType::False:
            advance();
            return JsonValue(false);
        case TokenType::Null:
            advance();
            return JsonValue(nullptr);
        default:
            throw ParseException("Unexpected token", token.line, token.column);
    }
}

JsonValue JsonParser::parse_object() {
    consume(TokenType::LeftBrace);
    
    JsonObject obj;
    
    if (match(TokenType::RightBrace)) {
        advance();
        return JsonValue(obj);
    }
    
    while (true) {
        const auto& key_token = peek();
        if (key_token.type != TokenType::String) {
            throw ParseException("Expected string key in object", key_token.line, key_token.column);
        }
        
        std::string key = key_token.value;
        advance();
        
        consume(TokenType::Colon);
        JsonValue value = parse_value();
        obj[key] = value;
        
        if (!match(TokenType::Comma)) break;
        advance();
    }
    
    consume(TokenType::RightBrace);
    return JsonValue(obj);
}

JsonValue JsonParser::parse_array() {
    consume(TokenType::LeftBracket);
    
    JsonArray arr;
    
    if (match(TokenType::RightBracket)) {
        advance();
        return JsonValue(arr);
    }
    
    while (true) {
        arr.push_back(parse_value());
        
        if (!match(TokenType::Comma)) break;
        advance();
    }
    
    consume(TokenType::RightBracket);
    return JsonValue(arr);
}

JsonValue JsonParser::parse_string() {
    const auto& token = peek();
    advance();
    return JsonValue(token.value);
}

JsonValue JsonParser::parse_number() {
    const auto& token = peek();
    advance();
    return JsonValue(std::stod(token.value));
}

void JsonParser::consume(TokenType expected) {
    if (!match(expected)) {
        const auto& token = peek();
        throw ParseException("Expected token not found", token.line, token.column);
    }
    advance();
}

JsonParser::Token JsonParser::peek() const {
    if (token_index_ >= tokens_.size()) {
        return tokens_.back();
    }
    return tokens_[token_index_];
}

JsonParser::Token JsonParser::advance() {
    Token token = peek();
    if (token_index_ < tokens_.size()) {
        token_index_++;
    }
    return token;
}

bool JsonParser::match(TokenType type) const {
    return peek().type == type;
}

} // namespace gvault
