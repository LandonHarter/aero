#include "include/lexer.h"
#include "include/token.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Creates new lexer
lexer_T* init_lexer(char* contents) {
    lexer_T* lexer = calloc(1, sizeof(struct LEXER_STRUCT)); // Allocate memory for lexer
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];

    return lexer;
}

// Advances lexer by 1 character
void lexer_advance(lexer_T* lexer) {
    // If char != null character
    if (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        lexer->i += 1; // Increment index
        lexer->c = lexer->contents[lexer->i];
    }
}

// Skip space/whitespace character
void lexer_skip_whitespace(lexer_T* lexer) {
    // ' ' = space and 10 = new line
    while (lexer->c == ' ' || lexer->c == 10) {
        lexer_advance(lexer); // Advance if empty
    }
}

token_T* lexer_get_next_token(lexer_T* lexer) {
    // \0 = null character
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        if (lexer->c == ' ' || lexer->c == 10) {
            lexer_skip_whitespace(lexer); // If empty skip space
        }

        if (isalnum(lexer->c)) {
            return lexer_collect_id(lexer);
        }

        // String token
        if (lexer->c == '"') {
            return lexer_collect_string(lexer);
        }

        // Determine the token being used
        switch (lexer->c) {
            case ';': 
                return lexer_advance_with_token(lexer, init_token(TOKEN_SEMI, lexer_get_current_char_as_string(lexer)));
                break;
            case '=': 
                return lexer_advance_with_token(lexer, init_token(TOKEN_EQUALS, lexer_get_current_char_as_string(lexer)));
                break;
            case '(': 
                return lexer_advance_with_token(lexer, init_token(TOKEN_LPAREN, lexer_get_current_char_as_string(lexer)));
                break;
            case ')': 
                return lexer_advance_with_token(lexer, init_token(TOKEN_RPAREN, lexer_get_current_char_as_string(lexer)));
                break;
        }

        return (void*)0;
    }
}

token_T* lexer_collect_string(lexer_T* lexer) {
    lexer_advance(lexer); // Skip opening quote

    char* value = calloc(1, sizeof(char)); // Allocate char[]
    value[0] = '\0'; // Initialize char[]

    // Append values inbetween quotes
    while (lexer->c != '"') {
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(TOKEN_STRING, value); // Return new token
}

token_T* lexer_advance_with_token(lexer_T* lexer, token_T* token) {
    lexer_advance(lexer);

    return token;
}

// Alloc and create string with lexer char
char* lexer_get_current_char_as_string(lexer_T* lexer) {
    char* str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';

    return str;
}

token_T* lexer_collect_id(lexer_T* lexer) {
    char* value = calloc(1, sizeof(char)); // Allocate char[]
    value[0] = '\0'; // Initialize char[]

    // Append values inbetween quotes
    while (isalnum(lexer->c)) {
        char* s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lexer_advance(lexer);
    }

    return init_token(TOKEN_ID, value); // Return new token
}