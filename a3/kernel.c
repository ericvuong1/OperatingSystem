#include<stdlib.h>
#include<stdio.h>

#include "shell.h"
#include "ram.h"
#include "cpu.h"
#include "pcb.h"
#include "helper.h"

int myinit(FILE *p) {
	PCB *pcb;
	int result;

	result = addToRAM(p);

	if (result>=0) {
		pcb = makePCB(p,result);
		if (pcb != NULL) {
			addToReady(pcb);
			return 1;
		}
	}

	return 0;
}

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
			result = runCPU(2);

			if (result == 1) {
				pcb->PC_page = pcb->PC_page++;
				
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
