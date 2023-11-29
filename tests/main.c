#include <stdio.h>
#include <stdlib.h>
#include "../lib/regex.h"

int main(int argc, char** argv) {
    pattern incorrect = regComp("abc(defghi", 11) ;
    pattern search = regComp("abc(def)ghi", 11) ;
    if (regRun("abcdefghi", 9, search)) {
        printf("success\n");
    } else {
        printf("failure\n");
    }
    regFree(search);
}
