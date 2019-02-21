#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "pcb.h"
#include "kernel.h"
#include "shell.h"

// public
int isCPUAvailable();
void loadToCPU(PCB *pcb);
int runCPU(int quanta, PCB *pcb);

typedef struct CPU
{
    FILE *IP;
    char IR[1000];
    int quanta;
} CPU;

CPU cpu = {NULL, "", QUANTA};

// void runCPU(int quanta);

int isCPUAvailable()
{
    return cpu.IP == NULL;
}

void loadToCPU(PCB *pcb)
{
    cpu.IP = pcb->PC;
    // printf("DEBUG: Loaded to CPU, using quanta=%d=======\n", QUANTA);
}

int runCPU(int quanta, PCB *pcb)
{
    int currentQuanta = 0;
    // printf("DEBUG: running CPU\n");
    while (currentQuanta < QUANTA)
    {
        if (fgets(cpu.IR, 999, cpu.IP) != NULL)
        {
            parse(cpu.IR);
        }
        if (feof(cpu.IP))
        {
            cpu.IP = NULL;
            return 1;
        }
        currentQuanta = currentQuanta + 1;
    }
    pcb->PC = cpu.IP;
    cpu.IP = NULL;
    return 0;

    // while (!feof(cpu.IP) && currentQuanta < QUANTA && fgets(cpu.IR, 999, cpu.IP))
    // {
    //     // printf("DEBUG: Running quanta %d with command: %s\n", currentQuanta, cpu.IR);
    //     parse(cpu.IR);
    //     currentQuanta = currentQuanta + 1;
    //     if (feof(cpu.IP))
    //     {
    //         cpu.IP = NULL;
    //         return 1;
    //     }
    // }

    // if (feof(cpu.IP))
    // {
    //     cpu.IP = NULL;
    //     return 1;
    // }

    // pcb->PC = cpu.IP;
    // cpu.IP = NULL;
    // return 0;
}