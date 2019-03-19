typedef struct PCB {
	FILE *PC;
	int start; // startAddressRAM
	struct PCB *next;

	int pageTable[10];
	int PC_page;
	int PC_offset;
	int pages_max;
} PCB;

void addToReady(PCB *p);
PCB *getFromReady();
PCB *makePCB(FILE *p, int startAddressRAM);
PCB *getHead();
void printPCB();
