#include "common.hpp"
#include <sstream>
#include <iomanip>

namespace gvault {

// JsonValue Constructors
JsonValue::JsonValue() : type_(Type::Null), value_(nullptr) {}

JsonValue::JsonValue(std::nullptr_t) : type_(Type::Null), value_(nullptr) {}

JsonValue::JsonValue(bool value) : type_(Type::Boolean), value_(value) {}

JsonValue::JsonValue(double value) : type_(Type::Number), value_(value) {}

JsonValue::JsonValue(const std::string& value) : type_(Type::String), value_(value) {}

JsonValue::JsonValue(std::string&& value) : type_(Type::String), value_(std::move(value)) {}

JsonValue::JsonValue(const JsonArray& value) : type_(Type::Array), value_(value) {}

JsonValue::JsonValue(JsonArray&& value) : type_(Type::Array), value_(std::move(value)) {}

JsonValue::JsonValue(const JsonObject& value) : type_(Type::Object), value_(value) {}

JsonValue::JsonValue(JsonObject&& value) : type_(Type::Object), value_(std::move(value)) {}

// Type checking
JsonValue::Type JsonValue::type() const {
    return type_;
}

bool JsonValue::is_null() const { return type_ == Type::Null; }
bool JsonValue::is_boolean() const { return type_ == Type::Boolean; }
bool JsonValue::is_number() const { return type_ == Type::Number; }
bool JsonValue::is_string() const { return type_ == Type::String; }
bool JsonValue::is_array() const { return type_ == Type::Array; }
bool JsonValue::is_object() const { return type_ == Type::Object; }

// Getters
bool JsonValue::as_boolean() const {
    if (!is_boolean()) throw std::runtime_error("Value is not a boolean");
    return std::get<bool>(value_);
}

double JsonValue::as_number() const {
    if (!is_number()) throw std::runtime_error("Value is not a number");
    return std::get<double>(value_);
}

const std::string& JsonValue::as_string() const {
    if (!is_string()) throw std::runtime_error("Value is not a string");
    return std::get<std::string>(value_);
}

const JsonArray& JsonValue::as_array() const {
    if (!is_array()) throw std::runtime_error("Value is not an array");
    return std::get<JsonArray>(value_);
}

const JsonObject& JsonValue::as_object() const {
    if (!is_object()) throw std::runtime_error("Value is not an object");
    return std::get<JsonObject>(value_);
}

// Mutators
JsonArray& JsonValue::as_array() {
    if (!is_array()) throw std::runtime_error("Value is not an array");
    return std::get<JsonArray>(value_);
}

JsonObject& JsonValue::as_object() {
    if (!is_object()) throw std::runtime_error("Value is not an object");
    return std::get<JsonObject>(value_);
}

// Utility
std::string JsonValue::to_string() const {
    std::stringstream ss;
    
    switch (type_) {
        case Type::Null:
            ss << "null";
            break;
        case Type::Boolean:
            ss << (std::get<bool>(value_) ? "true" : "false");
            break;
        case Type::Number:
            ss << std::setprecision(15) << std::get<double>(value_);
            break;
        case Type::String:
            ss << "\"" << std::get<std::string>(value_) << "\"";
            break;
        case Type::Array: {
            ss << "[";
            const auto& arr = std::get<JsonArray>(value_);
            for (size_t i = 0; i < arr.size(); ++i) {
                if (i > 0) ss << ",";
                ss << arr[i].to_string();
            }
            ss << "]";
            break;
        }
        case Type::Object: {
            ss << "{";
            const auto& obj = std::get<JsonObject>(value_);
            bool first = true;
            for (const auto& [key, val] : obj) {
                if (!first) ss << ",";
                ss << "\"" << key << "\":" << val.to_string();
                first = false;
            }
            ss << "}";
            break;
        }
    }
    
    return ss.str();
}

} // namespace gvault
