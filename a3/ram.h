// Public functions
//

void initRAM();
int addToRAM(FILE *p); // return startAddress or error codes
void clearRAM(int startAddressRAM);

// Public variables
//

extern FILE *ram[10];