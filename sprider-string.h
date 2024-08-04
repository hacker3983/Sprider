#ifndef _SPRIDER_STRING_H
#define _SPRIDER_STRING_H
#include <stdlib.h>
#include <string.h>

typedef struct sprider_stringlist {
    char** list;
    size_t count;
} sprider_stringlist_t;

// Get the sizeof a utf8 character by taking the first byte
int sprider_string_getutf8char_len(unsigned char first_byte);
// Appends a utf8 encoded character to the end of a char* string
void sprider_string_appendchar(char** dest, const char* utf8_char, size_t* dest_size);
// Appends a utf8 encoded character at a particular index within a char* string
void sprider_string_insertchar_atindex(char** dest, const char* utf8_char, size_t index, size_t* dest_size);


/* String list manipulation functions */
void sprider_stringlist_append(sprider_stringlist_t* string_list, const char* string_value);
void sprider_stringlist_destroy(sprider_stringlist_t* string_list);
#endif