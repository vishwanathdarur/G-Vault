#pragma once

#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <map>

namespace gvault {

// Forward declarations
class JsonValue;
using JsonObject = std::map<std::string, JsonValue>;
using JsonArray = std::vector<JsonValue>;

// JSON Value variant type
class JsonValue {
public:
    enum class Type {
        Null,
        Boolean,
        Number,
        String,
        Array,
        Object
    };

    // Constructors
    JsonValue();
    JsonValue(std::nullptr_t);
    JsonValue(bool value);
    JsonValue(double value);
    JsonValue(const std::string& value);
    JsonValue(std::string&& value);
    JsonValue(const JsonArray& value);
    JsonValue(JsonArray&& value);
    JsonValue(const JsonObject& value);
    JsonValue(JsonObject&& value);

    // Type checking
    Type type() const;
    bool is_null() const;
    bool is_boolean() const;
    bool is_number() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;

    // Getters
    bool as_boolean() const;
    double as_number() const;
    const std::string& as_string() const;
    const JsonArray& as_array() const;
    const JsonObject& as_object() const;

    // Mutators
    JsonArray& as_array();
    JsonObject& as_object();

    // Utility
    std::string to_string() const;

private:
    Type type_;
    std::variant<
        std::nullptr_t,
        bool,
        double,
        std::string,
        JsonArray,
        JsonObject
    > value_;
};

} // namespace gvault
