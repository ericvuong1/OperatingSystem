// typedef struct SHELLMEMORY
// {
//     char *variable;
//     char *value;
//     struct SHELLMEMORY *next;
// } shellMemory;

// shellMemory *containsVariable(char *words[]);
// void createVariable(char *words[]);
// void showShellMemory();

typedef struct PCB {
    FILE *PC;
    int ramCell;
} PCB;

PCB* makePCB(FILE *p, int ramCell);