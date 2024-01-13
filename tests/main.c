#include <stdio.h>
#include "../lib/regex.h"

int main(int argc, char** argv) {
    pattern incorrect = regComp("abc(defghi") ;
    pattern search = regComp("abc(def)ghi") ;
    if (regRun("abcdefghi", search)) {
        printf("success\n");
    } else {
        printf("failure\n");
    }
    regFree(search);
}
