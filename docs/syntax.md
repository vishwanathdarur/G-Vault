# Q-Vault Query Language (QQL) Syntax

The Q-Vault Query Language (QQL) is a minimal, strongly-typed, SQL-like language designed specifically for querying and filtering large JSON datasets.

## 1. Basic Structure
A standard query consists of a `SELECT` projection clause and an optional `WHERE` filtering clause.

```sql
SELECT <fields> WHERE <condition>
```

### Example
```sql
SELECT name, address.city 
WHERE age >= 21 AND active == true
```

## 2. Types and Literals
QQL supports the following primitive types (mapping directly to JSON types):
* **Strings:** `"Hello World"` (Must use double quotes)
* **Numbers:** `42`, `-3.14` (Integers and floats)
* **Booleans:** `true`, `false`
* **Null:** `null`

## 3. Identifiers (Paths)
Identifiers represent keys within the JSON data. Nested objects can be accessed using dot-notation.
* `age` -> Accesses `{"age": 25}`
* `user.address.zipcode` -> Accesses `{"user": {"address": {"zipcode": "12345"}}}`

## 4. Operators
QQL supports standard logical and comparison operators for the `WHERE` clause.

### Comparison Operators
* `==` : Equal to
* `!=` : Not equal to
* `>`  : Greater than
* `<`  : Less than
* `>=` : Greater than or equal to
* `<=` : Less than or equal to

### Logical Operators
* `AND` : Logical AND (Short-circuiting supported)
* `OR`  : Logical OR (Short-circuiting supported)
* `NOT` : Logical NOT

## 5. Formal Grammar (EBNF)

```ebnf
Query       ::= SelectClause [WhereClause]
SelectClause::= "SELECT" Identifier { "," Identifier }
WhereClause ::= "WHERE" Expression

Expression  ::= LogicalOr
LogicalOr   ::= LogicalAnd { "OR" LogicalAnd }
LogicalAnd  ::= Equality { "AND" Equality }
Equality    ::= Relational { ("==" | "!=") Relational }
Relational  ::= Primary { (">" | "<" | ">=" | "<=") Primary }

Primary     ::= Identifier | String | Number | Boolean | "null" | "(" Expression ")" | "NOT" Primary
Identifier  ::= [a-zA-Z_] { [a-zA-Z0-9_.] }
```
