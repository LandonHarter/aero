#include "include/lexer.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
    lexer_T* lexer = init_lexer(
        "var name = \"landon harter \";\n"
        "print(name);\n"
    );
    token_T* token = (void*)0;
    while ((token = lexer_get_next_token(lexer)) != (void*)0) {
        printf("Token Found: ID: %d, VALUE: %s \n", token->type, token->value);
    }

    return 0;
}