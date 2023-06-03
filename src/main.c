#include <stdio.h>
#include <stdlib.h>

#include "csv.h"

#define SEPARATOR ';'

int main(int argc, char** argv)
{
    FILE* csv_file = fopen("../test.csv", "r");
    if (!csv_file)
        return -1;

    size_t read_characters = 0;
    char* line = NULL;
    size_t line_length = 0;
    int number_of_keys = 0;
    const char separator = SEPARATOR;

    read_characters = getline(&line, &line_length, csv_file);
    if (read_characters == -1)
        return -1;

    Array keys = csv_parse_line(&line, read_characters, separator);
    char* values_json = NULL;
    json_stringify_array(&values_json, &keys);
    printf("Keys: %s\n\n", values_json);
    free(values_json);

    printf("Values\n------\n\n");

    Array object_list = {
        .size = 0,
        .data = malloc(4 * sizeof(Value))
    };

    while ((read_characters = getline(&line, &line_length, csv_file)) != -1) {
        Array values = csv_parse_line(&line, read_characters, separator);
        Object object = {
            .size = values.size,
            .pairs = malloc(values.size * sizeof(Pair))
        };
        for (size_t i = 0; i < values.size; i++) {
            Pair pair;
            strcpy(pair.key, keys.data[i].value.string);
            Value value = {
                .type = VALUE_TYPE_STRING
            };
            strcpy(value.value.string, values.data[i].value.string);
            pair.value = value; 
            object.pairs[i] = pair;
        }
        Value value = {
            .type = VALUE_TYPE_OBJECT,
            .value.object = object
        };
        object_list.data[object_list.size++] = value;
        free(values.data);
    }

    char* csv_json = NULL;

#ifdef TESTS
    // Tests
    // String Test
    Value string_value = {
        .type = VALUE_TYPE_STRING,
        .value.string = "test"
    };
    value_to_json(&csv_json, &string_value);
    printf("%s\n", csv_json);

    // Object Test
    Pair object_pair = {
        .key = "key",
        .value = string_value
    };
    Pair object_pairs[] = {object_pair};
    Value object_value = {
        .type = VALUE_TYPE_OBJECT,
        .value.object = {
            .size = 1,
            .pairs = object_pairs
        }
    };
    value_to_json(&csv_json, &object_value);
    printf("%s\n", csv_json);

    // String Array Test
    Array string_array = {
        .size = 1,
        .data = &string_value
    };
    array_to_json(&csv_json, &string_array);
    printf("%s\n", csv_json);

    // Object Array Test
    Array object_array = {
        .size = 1,
        .data = &object_value
    };
    array_to_json(&csv_json, &object_array);
    printf("%s\n", csv_json);
#endif


    json_stringify_array(&csv_json, &object_list);
    printf("%s\n", csv_json);

    free(keys.data);
    fclose(csv_file);

    return 0;
}
