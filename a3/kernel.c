#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "shell.h"
#include "ram.h"
#include "cpu.h"
#include "pcb.h"
#include "memorymanager.h"

FILE *ram[10];

void terminate(PCB *p)
{
	// clearRAM(p->start);
	free(p);
}

int pageFault(PCB *pcb)
{
	printf("DEBUG: Page Fault Requested\n");
	pcb->PC_page++;
	printf("DEBUG: Looking for page: %d\n", pcb->PC_page);
	int PC_page = pcb->PC_page;

	if (PC_page > pcb->pages_max - 1 || PC_page > 9)
	{
		printf("DEBUG: %s program finished\n", filePaths[pcb->start]);
		return 99;
	} // program finished

	else if (pcb->pageTable[PC_page] != -1)
	{ // valid, we have a frame number
		int frame = pcb->pageTable[PC_page];
		printf("DEBUG: Frame valid, is in RAM, Frame: %d\n", frame);
		FILE *PC = ram[frame];
		pcb->PC_offset = 0;
	}
	else
	{ // not valid
		FILE *f = fopen(filePaths[pcb->start], "r");
		FILE *page = findPage(PC_page, f);
		int frameNumber = findFrame(page);

		// find hole in ram
		if (frameNumber == -1)
		{
			printf("DEBUG: Frame Not found, looking for hole\n");
			for (int i = 0; i < 10; i++)
			{
				if (ram[i] == NULL)
				{
					frameNumber = i;
					printf("DEBUG: Hole found, Frame : %d\n", frameNumber);
					break;
				}
			}
		}

		int victimFrame = 0;
		if (frameNumber == -1)
		{
			printf("DEBUG: No holes found, finding victim frame\n");
			victimFrame = findVictim(pcb);
			printf("DEBUG: we have a victim Frame %d\n", victimFrame);
		}
		else
		{
			printf("DEBUG: Frame found! Frame: %d\n", frameNumber);
		}

		updateFrame(frameNumber, victimFrame, page);
		updatePageTable(pcb, PC_page, frameNumber, victimFrame);

		if (frameNumber == -1)
			pcb->PC = ram[victimFrame];
		else
			pcb->PC = ram[frameNumber];

		pcb->PC_offset = 0;
	}
	return 0;
}

void scheduler()
{
	PCB *pcb;
	int result;

	// initialize CPU
	initCPU();

	// execute the processes
	while (getHead() != NULL)
	{
		debugRAM();
		// printPCB(); // for debugging

		pcb = getFromReady();

		if (pcb != NULL)
		{
			setCPU(pcb->PC);

			setCPUOffset(pcb->PC_offset); // *

			result = runCPU(2);

			if (result == 0)
			{
				pcb->PC_offset += 2;
			}

			if (result == 1)
			{ // page fault occurs
				result = pageFault(pcb);
			}

			if (result == 99)
				terminate(pcb);
			else
				addToReady(pcb);
		}
	}
}

void prepareBackingStore()
{
	system("rm -rf BackingStore");
	system("mkdir BackingStore");
}

void boot()
{
	printf("DEBUG: Booting PC...\n");
	printf("DEBUG: Initialize RAM array to NULL\n");
	printf("DEBUG: Preparing Backing Store\n");
	for (int i = 0; i < 10; i++)
	{
		ram[i] = NULL;
	}
	prepareBackingStore();
}

int main()
{
	boot();

	int result = 0;

	initRAM();
	initCPU();

	result = shell();

	return result;
}
