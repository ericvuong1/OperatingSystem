void initIO();
int partition(char *name, int blocksize, int totalblocks); // create & format partition, 1 success 0 fail
int mount(char *name); // load FAT & create buffer_block 1 success 0 fail 
int openfile(char *name); // find filename or creates file if it does not exist, return file's FAT index
int writeBlock(int file, char *data);
FILE *getPtr(int ptr);