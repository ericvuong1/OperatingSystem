#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void initIO(); // init global data struct and variables
int partition(char *name, int blocksize, int totalblocks); // create & format partition, 1 success 0 fail
int mount(char *name); // load FAT & create buffer_block 1 success 0 fail 
int openfile(char *name); // find filename or creates file if it does not exist, return file's FAT index
int readBlock(int file); // using file FAT index number, load buffer with data from blockID
char *returnBlock(); // return block as string from buffer_block
int writeBlock(int file, char *data); // sensitive to block size, write data to disk at blockID

struct PARTITION {
    int total_blocks;
    int block_size;
} partit;

struct FAT {
    char *filename;
    int file_length;
    int blockPtrs[10];
    int current_location;
} fat[20];

char *block_buffer;
FILE *fp[5];

// Helper function to concat strings
char *concatStrings(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void initIO() {
    partit.block_size = 0;
    partit.total_blocks = 0;

    block_buffer = NULL;
    for(int i=0;i<5;i++) fp[i]=NULL;
    // for(int i=0;i<20;i++) fat[i]=NULL; ??
}

int partition(char *name, int blocksize, int totalblocks) {
    system("mkdir PARTITION");
    FILE * p = fopen(concatStrings("./PARTITION/", name), "w");
    partit.total_blocks = totalblocks;
    partit.block_size = blocksize;
    int totalFreeBlockList = totalblocks * blocksize;
    // append partition data area 
    char *freeBlockList = "";
    for(int i=0;i<totalFreeBlockList;i++) freeBlockList = concatStrings(freeBlockList, "0");
    // TODO: write information from struct partition?
    // TODO: write information from FAT?
    fprintf(p, "%s", freeBlockList);
    fclose(p);

    return 1;
}
