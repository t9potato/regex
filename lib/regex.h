#include "parser.c"
#include "match.c"

pattern regComp(char* pattern, u16 length);
bool regRun(char* string, u16 length, pattern search);
