#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum {
    ONE,
    OPTIONAL,
    ONE_OR_MORE,
    ZERO_OR_MORE,
} match;

typedef struct TOKEN{
    match state;
    char symbol;
} token;

union item{
    union item* v;
    token t;
};

typedef struct {
    char key;
    union item value;
} value;

value* parse(char* in, int in_len, int* out_len) {
    *out_len = 0;
    int max_len = 10;
    bool escaped = false;
    value* out = malloc(max_len*sizeof(value));
    for (int i; i<in_len; i++) {
        if (escaped) {
            out[*out_len].key = 'v';
            out[*out_len].value.t.state = ONE;
            out[*out_len].value.t.symbol = in[i];
            (*out_len)++;
            escaped = false;
        } else {
            switch (in[i]) {
                case '\\':
                    escaped = true;
                    break;
                case '(':
                    break;
                case ')':
                    break;
                case '+':
                    if (out[*out_len].value.t.state != ONE) {
                        fprintf(stderr, "Invalid syntax");
                        return NULL;
                    }
                    out[*out_len].value.t.state = ONE_OR_MORE;
                    break;
                case '*':
                    if (out[*out_len].value.t.state != ONE) {
                        fprintf(stderr, "Invalid syntax");
                        return NULL;
                    }
                    out[*out_len].value.t.state = ZERO_OR_MORE;
                    break;
                case '?':
                    if (out[*out_len].value.t.state != ONE) {
                        fprintf(stderr, "Invalid syntax");
                        return NULL;
                    }
                    out[*out_len].value.t.state = OPTIONAL;
                    break;
                case '.':
                    break;
                default:
                    out[*out_len].key = 'v';
                    out[*out_len].value.t.state = ONE;
                    out[*out_len].value.t.symbol = in[i];
                    (*out_len)++;
                    break;
            }
        }
        if ((*out_len) == max_len) {
            max_len *= 2;
            value* tmp = realloc(out, max_len*sizeof(value));
            if (tmp == NULL) {
                free(out);
                fprintf(stderr, "Failed to allocate memory: aborting");
                return NULL;
            }
            out = tmp;
        }
    }
    return out;
}
