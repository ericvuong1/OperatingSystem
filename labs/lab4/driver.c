#include <stdio.h>

int driver_put(char c, FILE *p) {
    int result;
    result = putc(c, p);
    return result;
}