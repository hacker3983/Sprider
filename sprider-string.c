#include "sprider-string.h"

int sprider_string_getutf8char_len(unsigned char first_byte) {
    if ((first_byte & 0x80) == 0x00) {
        return 1; // ASCII character (1 byte)
    } else if ((first_byte & 0xE0) == 0xC0) {
        return 2; // 2-byte UTF-8 character
    } else if ((first_byte & 0xF0) == 0xE0) {
        return 3; // 3-byte UTF-8 character
    } else if ((first_byte & 0xF8) == 0xF0) {
        return 4; // 4-byte UTF-8 character
    }
    return 0; // Invalid UTF-8 sequence
}


void sprider_string_appendchar(char** dest, const char* utf8_char, size_t* dest_size) {
    /*size_t utf8_charlen = strlen(utf8_char);
    size_t new_destlen = (*dest_size) + utf8_charlen;
    *dest = realloc(*dest, (new_destlen+1) * sizeof(char));
    (*dest_size) += utf8_charlen; 
    //strncpy();*/
}