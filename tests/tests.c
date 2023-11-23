#include <stdio.h>
#include <stdbool.h>

bool parseT(void);

int main(int argc, char** argv) {
    printf("Parser %b\n", parseT());
}

bool parseT(void) {
    return true;
}
