#include "json_parser.hpp"
#include <iostream>
#include <cassert>

using namespace gvault;

void test_parse_null() {
    JsonParser parser;
    JsonValue result = parser.parse("null");
    assert(result.is_null());
    std::cout << "✓ test_parse_null passed\n";
}

void test_parse_boolean() {
    JsonParser parser;
    
    JsonValue true_val = parser.parse("true");
    assert(true_val.is_boolean());
    assert(true_val.as_boolean() == true);
    
    JsonValue false_val = parser.parse("false");
    assert(false_val.is_boolean());
    assert(false_val.as_boolean() == false);
    
    std::cout << "✓ test_parse_boolean passed\n";
}

void test_parse_number() {
    JsonParser parser;
    
    JsonValue int_val = parser.parse("42");
    assert(int_val.is_number());
    assert(int_val.as_number() == 42.0);
    
    JsonValue float_val = parser.parse("3.14");
    assert(float_val.is_number());
    assert(std::abs(float_val.as_number() - 3.14) < 1e-10);
    
    JsonValue negative_val = parser.parse("-100");
    assert(negative_val.is_number());
    assert(negative_val.as_number() == -100.0);
    
    std::cout << "✓ test_parse_number passed\n";
}

void test_parse_string() {
    JsonParser parser;
    
    JsonValue str = parser.parse("\"hello world\"");
    assert(str.is_string());
    assert(str.as_string() == "hello world");
    
    JsonValue escaped = parser.parse("\"line1\\nline2\"");
    assert(escaped.is_string());
    assert(escaped.as_string() == "line1\nline2");
    
    std::cout << "✓ test_parse_string passed\n";
}

void test_parse_array() {
    JsonParser parser;
    
    JsonValue arr = parser.parse("[1, 2, 3]");
    assert(arr.is_array());
    const auto& array = arr.as_array();
    assert(array.size() == 3);
    assert(array[0].as_number() == 1.0);
    assert(array[1].as_number() == 2.0);
    assert(array[2].as_number() == 3.0);
    
    JsonValue empty_arr = parser.parse("[]");
    assert(empty_arr.is_array());
    assert(empty_arr.as_array().size() == 0);
    
    JsonValue mixed_arr = parser.parse("[1, \"two\", true, null]");
    assert(mixed_arr.is_array());
    const auto& mixed = mixed_arr.as_array();
    assert(mixed.size() == 4);
    assert(mixed[0].is_number());
    assert(mixed[1].is_string());
    assert(mixed[2].is_boolean());
    assert(mixed[3].is_null());
    
    std::cout << "✓ test_parse_array passed\n";
}

void test_parse_object() {
    JsonParser parser;
    
    JsonValue obj = parser.parse("{\"name\": \"Alice\", \"age\": 30}");
    assert(obj.is_object());
    const auto& object = obj.as_object();
    assert(object.size() == 2);
    assert(object.at("name").as_string() == "Alice");
    assert(object.at("age").as_number() == 30.0);
    
    JsonValue empty_obj = parser.parse("{}");
    assert(empty_obj.is_object());
    assert(empty_obj.as_object().size() == 0);
    
    std::cout << "✓ test_parse_object passed\n";
}

void test_parse_nested() {
    JsonParser parser;
    
    JsonValue nested = parser.parse(R"({
        "user": {
            "name": "Bob",
            "emails": ["bob@example.com", "robert@example.com"]
        },
        "active": true
    })");
    
    assert(nested.is_object());
    const auto& root = nested.as_object();
    assert(root.at("user").is_object());
    assert(root.at("user").as_object().at("name").as_string() == "Bob");
    assert(root.at("user").as_object().at("emails").is_array());
    assert(root.at("active").as_boolean() == true);
    
    std::cout << "✓ test_parse_nested passed\n";
}

void test_parse_scientific_notation() {
    JsonParser parser;
    
    JsonValue sci = parser.parse("1.23e-4");
    assert(sci.is_number());
    assert(std::abs(sci.as_number() - 0.000123) < 1e-10);
    
    std::cout << "✓ test_parse_scientific_notation passed\n";
}

void test_error_handling() {
    JsonParser parser;
    
    try {
        parser.parse("{invalid}");
        assert(false && "Should have thrown");
    } catch (const ParseException&) {
        std::cout << "✓ test_error_handling (invalid object) passed\n";
    }
    
    try {
        parser.parse("[1, 2,");
        assert(false && "Should have thrown");
    } catch (const ParseException&) {
        std::cout << "✓ test_error_handling (incomplete array) passed\n";
    }
}

int main() {
    std::cout << "Running CPU JSON Parser Tests...\n\n";
    
    try {
        test_parse_null();
        test_parse_boolean();
        test_parse_number();
        test_parse_string();
        test_parse_array();
        test_parse_object();
        test_parse_nested();
        test_parse_scientific_notation();
        test_error_handling();
        
        std::cout << "\n✅ All tests passed!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cout << "\n❌ Test failed: " << e.what() << std::endl;
        return 1;
    }
}
