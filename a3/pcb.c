#include<stdlib.h>
#include<stdio.h>
#include "pcb.h"
#include "memorymanager.h"

// Global data structures
//

PCB *head = NULL, *tail=NULL;

void printPCB() {
	PCB *temp = head;

	printf("Addr:");

	while(temp!=NULL) {
		printf(" %d",temp->start);
		temp = temp->next;
	}

	printf("\n");
}

PCB *makePCB(FILE *p, int startAddressRAM) {
	printf("DEBUG: Setting up PCB\n");
	PCB *pcb;

	if (p == NULL) return NULL;

	pcb = (PCB *) malloc(sizeof(PCB));
	if (pcb == NULL) return NULL;

	pcb->PC    = p;
	pcb->start = startAddressRAM;
	pcb->next  = NULL;
	pcb->PC_page = 0;
	pcb->PC_offset = 0;
	pcb->pages_max = countTotalPages(p);
	for(int i = 0; i < 10; i++) {
		pcb->pageTable[i] = -1; // invalid
	}
	printf("DEBUG: max pages: %d\n", pcb->pages_max);

	return pcb;
}

void addToReady(PCB *p) {
	p->next = NULL;

	if (head == NULL) {
		head = p;
		tail = p;
	} else {
		tail->next = p;
		tail = p;
	}
}

PCB *getHead() {
	return head;
}
PCB *getFromReady() {
	PCB *temp;

	if (head == NULL) return NULL;

	temp = head;
	head = head->next;
	temp->next = NULL;

	if (head == NULL) tail = NULL;

	return temp;
}

