#include "csv.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 100000000

Array csv_parse_line(char** in_line, size_t size, char separator)
{
    char* line = *in_line;
    size_t j = 0;
    String value_buffer;
    Array values = {
        .size = size > 0,
        .data = malloc(sizeof(String))
    };

    for (size_t i = 0; i < size; i++, j++) {
        bool char_is_separator = line[i] == separator;
        if (char_is_separator) {
            value_buffer[j] = '\0';
            strcpy(values.data[values.size - 1].value.string, value_buffer);
            values.size++;
            values.data = realloc(values.data, values.size * sizeof(String));
            j = -1;
            continue;
        }
        value_buffer[j] = line[i];
    }
    if (value_buffer[j - 1] == '\n')
        value_buffer[j - 1] = '\0';
    else
        value_buffer[j] = '\0';
    strcpy(values.data[values.size - 1].value.string, value_buffer);

    return values;
}

size_t json_stringify_string(char** out_buffer, String* in_value)
{
    *out_buffer = calloc(2 + sizeof *in_value, sizeof(char));
    char* buffer = *out_buffer;
    size_t offset = 0;

    buffer[offset++] = '"';
    strcpy(buffer + offset, *in_value);
    for (size_t i = 0; (*in_value)[i] != '\0'; i++, offset++)
        ;
    buffer[offset++] = '"';

    return offset;
}

size_t json_stringify_array(char** out_buffer, Array* in_values)
{
    *out_buffer = calloc(BUFFER_SIZE, sizeof(char));
    char* buffer = *out_buffer;
    size_t offset = 0;
    char* value_json;
    size_t value_json_size;

    buffer[offset++] = '[';
    value_json_size = json_stringify_value(&value_json, &in_values->data[0]);
    strcpy(buffer + offset, value_json);
    free(value_json);
    offset += value_json_size;
    for (size_t i = 1; i < in_values->size; i++) {
        buffer[offset++] = ',';
        value_json_size = json_stringify_value(&value_json, &in_values->data[i]);
        strcpy(buffer + offset, value_json);
        free(value_json);
        offset += value_json_size;
    }
    buffer[offset++] = ']';

    return offset;
}

size_t json_stringify_pair(char** out_buffer, Pair* in_pair) {
    *out_buffer = calloc(BUFFER_SIZE, sizeof(char));
    char* buffer = *out_buffer;
    size_t offset = 0;
    char* value_json;
    size_t value_json_size;

    value_json_size = json_stringify_string(&value_json, &in_pair->key);
    strcpy(buffer + offset, value_json);
    free(value_json);
    offset += value_json_size;
    buffer[offset++] = ':';
    value_json_size = json_stringify_value(&value_json, &in_pair->value);
    strcpy(buffer + offset, value_json);
    free(value_json);
    offset += value_json_size;

    // printf("pair buffer: %s\n", buffer);
    // printf("pair buffer size: %ld\n", offset);

    return offset;
}

size_t json_stringify_object(char** out_buffer, Object* in_object) {
    *out_buffer = calloc(BUFFER_SIZE, sizeof(char));

    if (in_object->size == 0) {
        char* empty_object_json = "{}";
        strcpy(*out_buffer, empty_object_json);
        return strlen(empty_object_json);
    }

    char* buffer = *out_buffer;
    size_t offset = 0;
    char* pair_json;
    size_t pair_json_size;

    // printf("converting object to json:\n");
    // printf("size: %ld\n", in_object->size);

    buffer[offset++] = '{';
    pair_json_size = json_stringify_pair(&pair_json, &in_object->pairs[0]);
    strcpy(buffer + offset, pair_json);
    free(pair_json);
    offset += pair_json_size;
    for (size_t i = 1; i < in_object->size; i++) {
        buffer[offset++] = ',';
        pair_json_size = json_stringify_pair(&pair_json, &in_object->pairs[i]);
        strcpy(buffer + offset, pair_json);
        free(pair_json);
        offset += pair_json_size;
    }
    buffer[offset++] = '}';

    // printf("object buffer: %s\n", buffer);
    // printf("object buffer size: %ld\n", offset);

    return offset;
}

size_t json_stringify_value(char** out_buffer, Value* in_value) {
    switch (in_value->type) {
        case VALUE_TYPE_OBJECT:
            return json_stringify_object(out_buffer, (Object*)&in_value->value);
            break;
        default:
            return json_stringify_string(out_buffer, (String*)&in_value->value);
            break;
    }
}
