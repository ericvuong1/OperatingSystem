#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ram.h"

// public
int addToRAM(FILE *p, char* fileName);
void freeRAM(int ramCell);
int isRAMFree();
int ramExists(char *fileName);

RAM *ram[RAM_SIZE];

int addToRAM(FILE *p, char *fileName)
{
    // look for a free ram

    for (int i = 0; i < RAM_SIZE; i++)
    {
        if (ram[i] == NULL)
        {
            // printf("DEBUG: ram[%d] is free, adding to it.\n", i);
            RAM *tmp = malloc(sizeof(RAM));
            tmp->RAM = p;
            tmp->fileName = fileName;

            ram[i] = tmp;
            return i;
        }
    }
    return -1;
}

int ramExists(char *fileName) {
    // printf("DEBUG: CHECKING IF FILENAME EXISTS\n");
    for(int i = 0; i < RAM_SIZE; i++) {
        // printf("DEBUG: %s fileName\n", fileName);
        if(ram[i] != NULL) {
            if(strcmp(ram[i]->fileName, fileName) == 0) {
                return 1;
            }
        }
    }
    return 0;
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