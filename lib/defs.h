#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

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

typedef struct pattern {
    union element* el;
    u16 len;
    struct pattern* tail;
} pattern;

typedef union element {
    struct {
        u64 high;
        u64 low;
        match match;
    }t;
   pattern e;
} element;


