#include <stdlib.h>
#include <stdio.h>
#include "defs.h"

void insert(element* item, char character);

typedef struct LENSTACK {
    u16 len;
    struct LENSTACK* tail;
} lenStack;

pattern regexComp(char* str, u16 len) {
    pattern out;
    out.len = 0;
    out.el = malloc(10*sizeof(element));
    out.tail = NULL;
    lenStack maxOutLen;
    maxOutLen.len = 10;
    maxOutLen.tail = NULL;
    pattern* head = &out;
    bool escaped = false;
    bool set = false;
    for (u16 i = 0; i < len; i++) {
        if (set) {
            if (escaped) {
            } else {
                switch (str[i]) {
                    case ']':
                        set = false;
                        break;
                    default:
                        if (str[i] >= 64) {
                            head->el[head->len].t.high |= 1<<(str[i]-64);
                        } else {
                            head->el[head->len].t.high |= 1<<(str[i]);
                        }
                        break;
                }
            }
        } else if (escaped) {
            head->el[++(head->len)].t.match = ONE;
            insert(&head->el[head->len], str[i]);
            escaped = false;
        } else {
            switch (str[i]) {
                case '.':
                    head->el[++(head->len)].t.match = ONE;
                    head->el[head->len].t.high = UINT64_MAX;
                    head->el[head->len].t.low = UINT64_MAX;
                    break;
//              case '*':
//                  if (head->el[head->len].t.match == ONE) {
//                      head->el[head->len].t.match = ZERO_OR_MORE;
//                  } else {
//                      goto fail;
//                  }
//                  break;
//              case '+':
//                  if (head->el[head->len].t.match == ONE) {
//                      head->el[head->len].t.match = ONE_OR_MORE;
//                  } else {
//                      goto fail;
//                  }
//                  break;
//              case '?':
//                  if (head->el[head->len].t.match == ONE) {
//                      head->el[head->len].t.match = OPTIONAL;
//                  } else {
//                      goto fail;
//                  }
//                  break;
//              case '\\':
//                  escaped = true;
//                  break;
                case '(':;
                    lenStack new;
                    new.len = 10;
                    new.tail = &maxOutLen;
                    maxOutLen = new;
                    pattern new_head;
                    new_head.el = malloc(10*sizeof(pattern));
                    new_head.len = 10;
                    new_head.tail = head;
                    head->el[++(head->len)].e = new_head;
                    head = &new_head;
                    break;
                case ')':
                    if (out.tail == NULL) {
                        goto fail;
                    }
                    head = head->tail;
                    lenStack* maxOutLen = (maxOutLen->tail);
                    break;
//              case '[':
//                  set = true;
//                  head->el[++(head->len)].t.match = ONE;
//                  break;
                default:
                    head->el[++(head->len)].t.match = ONE;
                    insert(&head->el[head->len], str[i]);
            }
        }
        if (out.len > maxOutLen.len-1) {
            maxOutLen.len *= 2;
            void* result = realloc(out.el, maxOutLen.len * sizeof(pattern));
            if (result == NULL) {
                goto fail;
            }
            out.el = result;
            
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
