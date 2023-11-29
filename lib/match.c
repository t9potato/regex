#include "regex.h"
#include "defs.h"

int regRun(char* string, u16 length, pattern search) {
    u16 index;
    stack sIndex;
    sIndex.tail = NULL;
    sIndex.len = 0;
    pattern *head = &search;
    for (index = 0; index < length; index++) {
        if (sIndex.len == head->len) {
            if (head->tail == NULL) {
                goto succede;
            }
            head = head->tail;
            sIndex = *sIndex.tail;
        }

    }
    return 0;

succede: {
        return index;
    }
}
