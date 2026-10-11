#include "json_parser.hpp"
#include "query_lexer.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace gvault;

void print_value(const JsonValue& value, int indent = 0) {
    std::string indent_str(indent * 2, ' ');
    
    switch (value.type()) {
        case JsonValue::Type::Null:
            std::cout << "null";
            break;
        case JsonValue::Type::Boolean:
            std::cout << (value.as_boolean() ? "true" : "false");
            break;
        case JsonValue::Type::Number:
            std::cout << value.as_number();
            break;
        case JsonValue::Type::String:
            std::cout << "\"" << value.as_string() << "\"";
            break;
        case JsonValue::Type::Array: {
            const auto& arr = value.as_array();
            if (arr.empty()) {
                std::cout << "[]";
            } else {
                std::cout << "[\n";
                for (size_t i = 0; i < arr.size(); ++i) {
                    std::cout << indent_str << "  ";
                    print_value(arr[i], indent + 1);
                    if (i < arr.size() - 1) std::cout << ",";
                    std::cout << "\n";
                }
                std::cout << indent_str << "]";
            }
            break;
        }
        case JsonValue::Type::Object: {
            const auto& obj = value.as_object();
            if (obj.empty()) {
                std::cout << "{}";
            } else {
                std::cout << "{\n";
                size_t i = 0;
                for (const auto& [key, val] : obj) {
                    std::cout << indent_str << "  \"" << key << "\": ";
                    print_value(val, indent + 1);
                    if (i < obj.size() - 1) std::cout << ",";
                    std::cout << "\n";
                    i++;
                }
                std::cout << indent_str << "}";
            }
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " <json_file>                Parse and display a JSON file\n"
                  << "  " << argv[0] << " --tokens \"<query>\"         Tokenize and display a QQL query\n";
        return 1;
    }

    std::string arg1 = argv[1];

    if (arg1 == "--tokens") {
        if (argc < 3) {
            std::cerr << "Error: --tokens requires a query string argument.\n"
                      << "Example: " << argv[0] << " --tokens \"SELECT name WHERE age > 20\"\n";
            return 1;
        }
        std::string query = argv[2];
        try {
            qvault::QueryLexer lexer(query);
            auto tokens = lexer.tokenize();
            std::cout << "Tokens (" << tokens.size() << " total):\n";
            for (const auto& token : tokens) {
                std::cout << "  " << token.to_string() << "\n";
            }
            return 0;
        } catch (const qvault::LexerException& e) {
            std::cerr << "Lexer error at line " << e.line() 
                      << ", column " << e.column() << ": " 
                      << e.what() << "\n";
            return 1;
        }
    }
    
    try {
        JsonParser parser;
        JsonValue parsed = parser.parse_file(argv[1]);
        
        std::cout << "Parsed JSON:\n";
        print_value(parsed);
        std::cout << "\n";
        
        return 0;
    } catch (const ParseException& e) {
        std::cerr << "Parse error at line " << e.line() 
                  << ", column " << e.column() << ": " 
                  << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
