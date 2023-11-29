#include "regex.h"
#include "defs.h"

void insert(element* item, char character);

pattern regComp(char* str, u16 len) {
    pattern out;
    out.len = 0;
    out.el = malloc(10*sizeof(element));
    out.tail = NULL;
    stack maxOutLen;
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
                    case ']': {
                        set = false;
                        head->len++;
                        break;
                    }
                    // case '-': {}
                    default: {
                        if (str[i] >= 64) {
                            head->el[head->len].value.t.low |= 1<<(str[i]-64);
                        } else {
                            head->el[head->len].value.t.high |= 1<<(str[i]);
                        }
                        break;
                    }
                }
            }
        } else if (escaped) {
            head->el[head->len].value.t.match = ONE;
            head->el[head->len].type = ELE;
            insert(&head->el[head->len], str[i]);
            head->len++;
            escaped = false;
        } else {
            switch (str[i]) {
                case '.': {
                    head->el[head->len].value.t.match = ONE;
                    head->el[head->len].type = ELE;
                    head->el[head->len].value.t.high = UINT64_MAX;
                    head->el[head->len].value.t.low = UINT64_MAX;
                    head->len++;
                    break;
                }
                case '*': {
                    u16 len = head->len - 1;
                    if (head->el[len].value.t.match == ONE) {
                        head->el[len].value.t.match = ZERO_OR_MORE;
                    } else {
                        fprintf(stderr, "Unable to parse regex - invalid use of length modifier:\n%s\n", str);
                        goto fail;
                    }
                    break;
                }
                case '+': {
                    u16 len = head->len - 1;
                    if (head->el[len].value.t.match == ONE) {
                        head->el[len].value.t.match = ONE_OR_MORE;
                    } else {
                        fprintf(stderr, "Unable to parse regex - invalid use of length modifier:\n%s\n", str);
                        goto fail;
                    }
                    break;
                }
                case '?': {
                    u16 len = head->len - 1;
                    if (head->el[len].value.t.match == ONE) {
                        head->el[len].value.t.match = OPTIONAL;
                    } else {
                        fprintf(stderr, "Unable to parse regex - invalid use of length modifier:\n%s\n", str);
                        goto fail;
                    }
                    break;
                }
                case '\\': {
                    escaped = true;
                    break;
                }
                case '(': {
                    stack new;
                    new.len = 10;
                    new.tail = &maxOutLen;
                    maxOutLen = new;
                    pattern new_head;
                    new_head.el = malloc(10*sizeof(pattern));
                    new_head.len = 0;
                    new_head.tail = head;
                    head->el[head->len].type = PAT;
                    head->el[head->len].value.e = new_head;
                    head->len++;
                    head = &new_head;
                    break;
                }
                case ')': {
                    if (head->tail == NULL) {
                        fprintf(stderr, "Unable to parse regex - unable to exit out of unopen subquery:\n%s\n", str);
                        goto fail;
                    }
                    head = head->tail;
                    maxOutLen = *(maxOutLen.tail);
                    break;
                }
                case '[': {
                    set = true;
                    head->el[head->len].type = ELE;
                    head->el[head->len].value.t.match = ONE;
                    head->len++;
                    break;
                }
                default: {
                    head->el[head->len].type = ELE;
                    head->el[head->len].value.t.match = ONE;
                    insert(&head->el[head->len], str[i]);
                    head->len++;
                }
            }
        }
        if (out.len > maxOutLen.len-1) {
            maxOutLen.len *= 2;
            void* result = realloc(out.el, maxOutLen.len * sizeof(pattern));
            if (result == NULL) {
                fprintf(stderr, "Unable to allocate proper memory");
                goto fail;
            }
            out.el = result;
            
        }
    }
    if (head->tail != NULL) {
        fprintf(stderr, "Unable to parse regex - unclosed expressions:\n%s\n", str);
        goto fail;
    }
    return out;
fail: {
    regFree(out);
    pattern tmp;
    tmp.len = 0;
    tmp.el = NULL;
    return tmp;
    }
}

void insert(element* e, char c) {
    if (c >= 64) {
        e->value.t.high =  1<<(c - 64);
        e->value.t.low = 0;
    } else {
        e->value.t.high = 0;
        e->value.t.low =  1<<(c);
    }
}

void regFree(pattern el) {
    for (u16 i = 0; i < el.len; i++) {
        if (el.el[i].type == PAT) {
            regFree(el.el[i].value.e);
        }
    }
    free(el.el);
}
