#include <stdlib.h>

typedef enum {
    ONE,
    ONE_OR_MORE,
    ZERO_OR_MORE,
} match;

typedef struct TOKEN{
    match state;
    char symbol;
} token;

typedef union value value;
union value{
    value* v;
    token t;
};

char* parse(char* input, int input_lent) {
    return NULL;
}
