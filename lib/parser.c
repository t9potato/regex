#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef enum {
    ONE,
    ZERO_OR_MORE,
    OPTIONAL,
    ONE_OR_MORE
} match;

typedef struct {
    union element* el;
    u16 len;
} pattern;

typedef union element {
    struct {
        u64 high;
        u64 low;
        match match;
    }t;
   pattern e;
} element;

void insert(element* item, char character);

typedef struct LENSTACK {
    u16 len;
    struct LENSTACK* tail;
} lenStack;

pattern parse(char* str, u16 len) {
    pattern out;
    out.len = 0;
    out.el = malloc(10*sizeof(element));
    lenStack maxOutLen;
    maxOutLen.len = 10;
    maxOutLen.tail = NULL;
    pattern* head = &out;
    bool escaped = false;
    for (u16 i = 0; i < len; i++) {
        if (escaped) {
            head->el[++(head->len)].t.match = ONE;
            insert(&head->el[head->len], str[i]);
            escaped = false;
        } else
        switch (str[i]) {
            case '.':
                head->el[++(head->len)].t.match = ONE;
                head->el[head->len].t.high = UINT64_MAX;
                head->el[head->len].t.low = UINT64_MAX;
                break;
            case '*':
                if (head->el[head->len].t.match == ONE)
                    head->el[head->len].t.match = ZERO_OR_MORE;
                else
                    goto fail;
                break;
            case '+':
                if (head->el[head->len].t.match == ONE)
                    head->el[head->len].t.match = ONE_OR_MORE;
                else
                    goto fail;
                break;
            case '?':
                if (head->el[head->len].t.match == ONE)
                    head->el[head->len].t.match = OPTIONAL;
                else
                    goto fail;
                break;
            case '\\':
                escaped = true;
                break;
            case '[':;
                lenStack new;
                new.len = 10;
                new.tail = &maxOutLen;
                maxOutLen = new;
                pattern new_head;
                head->el[++(head->len)].e = new_head;
                break;
            case ']':
                break;
            default:
                head->el[++(head->len)].t.match = ONE;
                insert(&head->el[head->len], str[i]);
        }

    }
    return out;
fail:;
    pattern tmp;
    tmp.len = 0;
    tmp.el = NULL;
    return tmp;
}

void insert(element* e, char c) {
    if (c >= 64) {
        e->t.high =  1<<(c - 64);
        e->t.low = 0;
    } else {
        e->t.high = 0;
        e->t.low =  1<<(c);
    }
}
