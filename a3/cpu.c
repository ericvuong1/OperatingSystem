#include <stdio.h>

#include "shell.h"
#include "pcb.h"

// Global data structures simulating hardware
//

struct CPU {
	FILE *IP;
	char IR[1000];
	int quanta;

	int offset;
} cpu;

void initCPU() {
	cpu.quanta = 2;
	cpu.IP     = NULL;
	cpu.IR[0]  = '\0';
}

void setCPU(FILE *PC) {
	cpu.IP    = PC;
	cpu.IR[0] = '\0';
}

void setCPUOffset(int PC_offset) {
	cpu.offset = PC_offset;
}

int runCPU(int quanta) {
	int result;
	char *p;

	cpu.quanta = quanta;

	while(cpu.quanta > 0) {
		if (cpu.offset == 4) {
			cpu.offset = 0;
			return 1; //page fault happens
		}
		
		p = fgets(cpu.IR, 999, cpu.IP);

		if (p == NULL || feof(cpu.IP)) return 99; // end of program

		printf(">>>%s\n",cpu.IR); // debug code

		result = prompt(cpu.IR);
		if (result != 0) return result;

		cpu.quanta--;

		cpu.offset++; 
	}	

	return 0; // no errors
}
