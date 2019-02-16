void myinit(FILE *p);
void scheduler();

typedef struct readyQueue
{
    struct PCB *pcb;
    struct readyQueue *next;
} readyQueue;

#define QUANTA 2