#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pcb.h"

PCB* makePCB(FILE *p);

PCB* makePCB(FILE *p) {
    printf("DEBUG: Making a new PCB...\n");
    PCB *pcb = malloc(sizeof(PCB));
    pcb -> PC = p;
    
    return pcb;
}