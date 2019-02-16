#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"
#include "shell.h"
#include "ram.h"
#include "pcb.h"
#include "kernel.h"
#include "cpu.h"

void myinit(FILE *p);
void addToReady(PCB *pcb);

readyQueue *readyQueueHead = NULL;
readyQueue *readyQueueTail = NULL;

void addToReady(PCB *pcb) {
    printf("DEBUG: Adding to ready queue...\n");
    readyQueue *tmp = malloc(sizeof(readyQueue));
    tmp->pcb = pcb;
    tmp->next = NULL;

    // case empty queue
    if (readyQueueHead == NULL) {
        readyQueueHead = tmp;
        readyQueueTail = tmp;
    } 
    // case one element queue 
    else if (readyQueueHead->next == NULL) {
        readyQueueTail = tmp;
        readyQueueHead->next = readyQueueTail;
    } 
    // case more than one element queue
    else {
        readyQueueTail->next = tmp;
        readyQueueTail = tmp;
    }
    printf("DEBUG: QUEUED A PCB\n");
    return;
}

int isReadyQueueEmpty() {
    return readyQueueHead == NULL;
}


readyQueue *popReadyQueue() {
    if (isReadyQueueEmpty()) {
        return NULL;
    }
    readyQueue *tmp = readyQueueHead;
    
    if (readyQueueHead == readyQueueTail) {
        readyQueueHead = NULL;
        readyQueueTail = NULL;
    } else {
        readyQueueHead = readyQueueHead->next;
    }
    printf("DEBUG: POPPED A PCB\n");
    return tmp;
}

void myinit(FILE *p) {
    addToRAM(p);
    PCB *pcb = makePCB(p);
    addToReady(pcb);
}

void scheduler() {
    while(!isReadyQueueEmpty())
    {
        if(isCPUAvailable()) {
            readyQueue *currentPCBQueue = popReadyQueue();
            if (currentPCBQueue == NULL)
                continue;
            
            loadToCPU(currentPCBQueue->pcb);

            int finished = runCPU(QUANTA, currentPCBQueue->pcb);

            if (!finished) {
                printf("DEBUG: CPU not finished with process, not EOF\n");
                addToReady(currentPCBQueue->pcb);
            } else {
                printf("DEBUG: CPU finished with process, reached EOF\n");
                free(currentPCBQueue);
            }
            printf("DEBUG: finished with a PCB =======\n");
        }
    }
    printf("DEBUG: Ready Queue is done!\n");
}

int main()
{
    printf("Kernel 1.0 loaded!\n");
    printf("Welcome to the Eric Vuong shell!\n");
    printf("Shell version 2.0 Update February 2019\n");

    char prompt[100] = {'$', '\0'};
    char userInput[1000];
    int errorCode = 0;
    while (errorCode == 0 && !feof(stdin))
    {
        printf("%s", prompt);
        if (fgets(userInput, 999, stdin) == NULL)
            continue;

        errorCode = parse(userInput);
    }

    return 0;
}
