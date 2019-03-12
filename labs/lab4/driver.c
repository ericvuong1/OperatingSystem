#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int driver_put(char c, FILE *p);
FILE *driver_open(char *filename);

int driver_put(char c, FILE *p)
{
    int result;
    result = putc(c, p);
    return result;
}

FILE *driver_open(char *filename)
{
    FILE *p;
    p = fopen(filename, "wt");
    return p;
}

int driver_fclose(FILE *f) {
    fclose(f);
    return 0;
}