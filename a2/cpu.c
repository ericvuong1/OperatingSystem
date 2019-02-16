#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "pcb.h"
#include "kernel.h"
// #include "shell.h"

// public
int isCPUAvailable();
void loadToCPU(PCB *pcb);

typedef struct CPU {
    FILE *IP;
    char IR[1000];
    int quanta;
} CPU;

CPU cpu = {NULL, "", QUANTA};

// void runCPU(int quanta);

int isCPUAvailable() {
    return cpu.IP == NULL;
}

void loadToCPU(PCB *pcb) {
    cpu.IP = pcb->PC;
    printf("DEBUG: Loaded to CPU, using quanta=%d\n", QUANTA);
}

// void runCPU(int quanta) {
//     int currentQuanta = 0;

//     while (!feof(cpu.IP) && fgets(cpu.IR, 999, cpu.IP) && currentQuanta < 2) {
//         parse(cpu.IR);
//         printf("DEBUG: Running quanta %d with command: %s", currentQuanta, cpu.IR);
//         currentQuanta = currentQuanta + 1;
//         // reached end of file
//         if(feof(cpu.IP)) {
            
//         }
//     }
//     // reached end of file
//     if (feof(cpu.IP)) {

//     }
//     else {
//         cpu.IP = NULL;
//     }
// }