#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// public
void addToRAM(FILE *p);

FILE *ram[10];

void addToRAM(FILE *p) {
    // look for a free ram
    int ram_size = sizeof(ram) / sizeof(ram[0]);
    
    for(int i = 0; i < ram_size; i++) {
        if (ram[i] == NULL) {
            printf("DEBUG: ram[%d] is free, adding to it.\n", i);
            ram[i] = p;
            break;
        }
    }
}