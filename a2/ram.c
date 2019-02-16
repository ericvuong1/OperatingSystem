#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ram.h"

// public
int addToRAM(FILE *p);
int isRAMFree();

FILE *ram[RAM_SIZE];

int addToRAM(FILE *p)
{
    // look for a free ram
    int ram_size = sizeof(ram) / sizeof(ram[0]);

    for (int i = 0; i < ram_size; i++)
    {
        if (ram[i] == NULL)
        {
            printf("DEBUG: ram[%d] is free, adding to it.\n", i);
            ram[i] = p;
            return i;
        }
    }
    return -1;
}
void freeRAM(int ramCell)
{
    if (ramCell < RAM_SIZE)
    {
        ram[ramCell] = NULL;
    }
}
int isRAMFree()
{
    for (int i = 0; i < RAM_SIZE; i++)
    {
        if (ram[i] != NULL)
        {
            return 0;
        }
    }
    return 1;
}