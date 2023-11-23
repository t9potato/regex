#include <stdio.h>
#include "../lib/regex.h"

int main(int argc, char** argv) {
    pattern search = regComp("abc(def)ghi", 11) ;
    if (regRun("abcdefghi", 9, search)) {
        printf("success");
    } else {
        printf("failure");
    }
}
