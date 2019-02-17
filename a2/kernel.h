void myInit(FILE *p, char *fileName);
void scheduler();

typedef struct readyQueue
{
    struct PCB *pcb;
    struct readyQueue *next;
} readyQueue;

#define QUANTA 2