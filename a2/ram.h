int addToRAM(FILE *p, char* fileName);
void freeRAM(int ramCell);
int isRAMFree();
int ramExists(char *fileName);
#define RAM_SIZE 10

typedef struct RAM
{
    FILE *RAM;
    char *fileName;
} RAM;

