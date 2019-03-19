#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>

#include "shell.h"
#include "ram.h"
#include "cpu.h"
#include "pcb.h"
#include "helper.h"
#include "memorymanager.h"

FILE *ram[10];

// int myinit(FILE *p) {
// 	PCB *pcb;
// 	int result = 0; // TODO: remove?

// 	pcb = makePCB(p,result);
// 	if (pcb != NULL) {
// 		addToReady(pcb);
// 		return 1;
// 	}

// 	return 0;
// }

void terminate(PCB *p) {
	clearRAM(p->start);
	free(p);
}

void scheduler() {
	PCB *pcb;
	int result;

	// initialize CPU
	initCPU();

	// execute the processes
	while(getHead() != NULL) {
		// printPCB(); // for debugging

		pcb = getFromReady();

		if (pcb != NULL) {
			setCPU(pcb->PC);

			setCPUOffset(pcb->PC_offset); // *

			result = runCPU(2);

			if(result == 0) {
				pcb->PC_offset += 2;
			}

			if (result == 1) { // page fault occurs
				pcb->PC_page++;
				printf("Looking for page: %d\n", pcb->PC_page);
				int PC_page = pcb->PC_page;

				if (PC_page > pcb->pages_max) {
					debug("TERMINATED??");
					terminate(pcb);
				} // program finished

				else if (pcb->pageTable[PC_page] != -1) { // valid, we have a frame number
					debug("VALID");
					int frame = pcb->pageTable[PC_page];
					printf("DEBUG: FRAME %d\n", frame);
					FILE *PC = ram[frame];
					pcb->PC_offset = 0;
				} else { // not valid
					FILE *f = fopen(filePaths[pcb->start], "r");
					FILE *page = findPage(PC_page, f);
					int frameNumber = findFrame(page);

					// find hole in ram
					debugRAM();
					printf("THE TRUTH: %d\n", frameNumber);
					if (frameNumber == -1) {
						for(int i = 0; i < 10; i++) {
							if (ram[i] == NULL) {
								frameNumber = i;
								break;
							}
						}
					}
					printf("PAGE FAULT FRAME NUMBER: %d\n", frameNumber);

					int victimFrame = 0;
					if(frameNumber == -1) {
						victimFrame = findVictim(pcb);
					}

					updateFrame(frameNumber, victimFrame, page);
					updatePageTable(pcb, PC_page, frameNumber, victimFrame);
					pcb->PC = ram[frameNumber];
					pcb->PC_offset = 0;
				}
			}

			if (result == 99) terminate(pcb);
			else addToReady(pcb);
		} 	
	}
}

void prepareBackingStore() {
	system("rm -rf BackingStore");
	system("mkdir BackingStore");
}

void boot() {
	debug("Booting PC...");
	debug("Initialize RAM array to NULL");
	debug("Preparing Backing Store");
	for(int i = 0; i < 10; i++) {
		ram[i] = NULL;
	}
	prepareBackingStore();
}

int main() {
	boot();

	int result = 0;

	initRAM();
	initCPU();

	result = shell();

	return result;
}
