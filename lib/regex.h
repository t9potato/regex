#ifndef __REGEX_H__
#define __REGEX_H__

#include <sys/types.h>
#include <stdint.h>

typedef enum {
    ONE,
    ZERO_OR_MORE,
    OPTIONAL,
    ONE_OR_MORE
} match;

typedef struct pattern {
    struct element* el;
    uint16_t len;
    struct pattern* tail;
} pattern;

typedef struct element {
    enum {
        ELE,
        PAT
    } type;
    union {
        struct {
            uint64_t high;
            uint64_t low;
            match match;
        }t;
        pattern e;
    } value;
} element;


pattern regComp(char* pattern, uint16_t length);
int regRun(char* string, uint16_t length, pattern search);
void regFree(pattern);
#endif
