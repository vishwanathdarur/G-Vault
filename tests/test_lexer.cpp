#include "query_lexer.hpp"
#include <iostream>
#include <cassert>

void test_basic_tokens() {
    std::string query = "SELECT name, age WHERE age >= 21 AND active == true";
    qvault::QueryLexer lexer(query);
    auto tokens = lexer.tokenize();

    assert(tokens.size() == 13);
    assert(tokens[0].type == qvault::TokenType::Select);
    assert(tokens[1].type == qvault::TokenType::Identifier && tokens[1].lexeme == "name");
    assert(tokens[2].type == qvault::TokenType::Comma);
    assert(tokens[3].type == qvault::TokenType::Identifier && tokens[3].lexeme == "age");
    assert(tokens[4].type == qvault::TokenType::Where);
    assert(tokens[5].type == qvault::TokenType::Identifier && tokens[5].lexeme == "age");
    assert(tokens[6].type == qvault::TokenType::GreaterOrEqual);
    assert(tokens[7].type == qvault::TokenType::Number && tokens[7].lexeme == "21");
    assert(tokens[8].type == qvault::TokenType::And);
    assert(tokens[9].type == qvault::TokenType::Identifier && tokens[9].lexeme == "active");
    assert(tokens[10].type == qvault::TokenType::Equal);
    assert(tokens[11].type == qvault::TokenType::True);
    assert(tokens[12].type == qvault::TokenType::EndOfFile);

    std::cout << "[PASS] test_basic_tokens\n";
}

void test_nested_path() {
    std::string query = "SELECT user.profile.name WHERE user.stats.score > 98.5";
    qvault::QueryLexer lexer(query);
    auto tokens = lexer.tokenize();

    assert(tokens[1].type == qvault::TokenType::Identifier && tokens[1].lexeme == "user.profile.name");
    assert(tokens[3].type == qvault::TokenType::Identifier && tokens[3].lexeme == "user.stats.score");
    assert(tokens[4].type == qvault::TokenType::GreaterThan);
    assert(tokens[5].type == qvault::TokenType::Number && tokens[5].lexeme == "98.5");

    std::cout << "[PASS] test_nested_path\n";
}

void test_strings_and_escapes() {
    std::string query = "SELECT name WHERE city == \"New York\" OR tag == 'special\\tvalue'";
    qvault::QueryLexer lexer(query);
    auto tokens = lexer.tokenize();

    assert(tokens[3].type == qvault::TokenType::Identifier && tokens[3].lexeme == "city");
    assert(tokens[4].type == qvault::TokenType::Equal);
    assert(tokens[5].type == qvault::TokenType::String && tokens[5].lexeme == "New York");
    assert(tokens[6].type == qvault::TokenType::Or);
    assert(tokens[7].type == qvault::TokenType::Identifier && tokens[7].lexeme == "tag");
    assert(tokens[8].type == qvault::TokenType::Equal);
    assert(tokens[9].type == qvault::TokenType::String && tokens[9].lexeme == "special\tvalue");

    std::cout << "[PASS] test_strings_and_escapes\n";
}

void test_comments_and_whitespace() {
    std::string query = "-- Fetch adults\nSELECT name // inline comment\nWHERE age > 18";
    qvault::QueryLexer lexer(query);
    auto tokens = lexer.tokenize();

    assert(tokens[0].type == qvault::TokenType::Select);
    assert(tokens[1].type == qvault::TokenType::Identifier && tokens[1].lexeme == "name");
    assert(tokens[2].type == qvault::TokenType::Where);
    assert(tokens[3].type == qvault::TokenType::Identifier && tokens[3].lexeme == "age");
    assert(tokens[4].type == qvault::TokenType::GreaterThan);
    assert(tokens[5].type == qvault::TokenType::Number && tokens[5].lexeme == "18");

    std::cout << "[PASS] test_comments_and_whitespace\n";
}

void test_error_handling() {
    bool caught_single_equal = false;
    try {
        qvault::QueryLexer lexer("SELECT name WHERE age = 20");
        lexer.tokenize();
    } catch (const qvault::LexerException& e) {
        caught_single_equal = true;
    }
    assert(caught_single_equal);

    bool caught_unterminated_str = false;
    try {
        qvault::QueryLexer lexer("SELECT name WHERE city == \"Unterminated");
        lexer.tokenize();
    } catch (const qvault::LexerException& e) {
        caught_unterminated_str = true;
    }
    assert(caught_unterminated_str);

    std::cout << "[PASS] test_error_handling\n";
}

int main() {
    std::cout << "Running QueryLexer tests...\n";
    test_basic_tokens();
    test_nested_path();
    test_strings_and_escapes();
    test_comments_and_whitespace();
    test_error_handling();
    std::cout << "All QueryLexer tests passed successfully!\n";
    return 0;
}
