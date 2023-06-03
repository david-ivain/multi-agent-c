#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define VALUE_SIZE 32

typedef char String[VALUE_SIZE];
typedef String Key;

typedef enum {
    VALUE_TYPE_STRING,
    VALUE_TYPE_OBJECT
} ValueType;

struct Object;
struct Pair;
struct Value;
struct Array;

typedef struct Object {
    size_t size;
    struct Pair* pairs;
} Object;

typedef struct Array {
    size_t size;
    struct Value* data;
} Array;

typedef struct Value {
    ValueType type;
    union {
        String string;
        Object object;
    } value;
} Value;

typedef struct Pair {
    Key key;
    Value value;
} Pair;

Array csv_parse_line(char** in_line, size_t size, char separator);

size_t json_stringify_string(char** out_buffer, String* in_value);

size_t json_stringify_array(char** out_buffer, Array* in_values);

size_t json_stringify_pair(char** out_buffer, Pair* in_pair);

size_t json_stringify_object(char** out_buffer, Object* in_object);

size_t json_stringify_value(char** out_buffer, Value* in_value);
