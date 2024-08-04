#include "sprider-string.h"

void sprider_stringlist_append(sprider_stringlist_t* string_list, const char* string_value) {
    if(!string_value) {
        return;
    }
    size_t string_value_length = strlen(string_value);
    if(!string_list->list) {
        string_list->list = malloc(sizeof(char*));
    } else {
        string_list->list = realloc(string_list->list, (string_list->count+1) * sizeof(char*));
    }
    string_list->list[string_list->count] = calloc(string_value_length+1, sizeof(char));
    strncpy(string_list->list[string_list->count], string_value, string_value_length);
    string_list->count++;
}

void sprider_stringlist_destroy(sprider_stringlist_t* string_list) {
    for(size_t i=0;i<string_list->count;i++) {
        free(string_list->list[i]); string_list->list[i] = NULL;
    }
    free(string_list->list); string_list->list = NULL;
    string_list->count = 0;
}