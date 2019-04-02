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

struct FILE_DS {
    int fatIndex;
} files[5];

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
}

int partition(char *name, int blocksize, int totalblocks) {
    system("mkdir PARTITION");
    FILE * p = fopen(concatStrings("./PARTITION/", name), "w");
    partit.total_blocks = totalblocks;
    partit.block_size = blocksize;
    int totalFreeBlockList = totalblocks * blocksize;
    // append partition data area 
    char *freeBlockList = "";
    fprintf(p, "%d\n", blocksize);
    fprintf(p, "%d\n", totalblocks);
    for(int i=0;i<totalFreeBlockList;i++) freeBlockList = concatStrings(freeBlockList, "0");
    // TODO: write information from struct partition?
    // TODO: write information from FAT?
    for(int i=0;i<20;i++){
        fprintf(p, "NULL\n");
        fprintf(p, "NULL\n");
        fprintf(p, "NULL\n");
        fprintf(p, "NULL\n");
    }
    fprintf(p, "%s\n", freeBlockList);
    fclose(p);

    return 1;
}
// TODO: not done
int mount(char *name) {
    FILE *p = fopen(concatStrings("./PARTITION/", name), "r");
    char buf[100];

    fgets(buf, 100, p);
    partit.block_size = atoi(buf);

    fgets(buf, 100, p);
    partit.total_blocks = atoi(buf);
    for(int i=0;i<20;i++){
        fgets(buf, 100,p);
        if(strcmp("NULL\n", buf)==0){ // skip if null
            fgets(buf,100,p);
            fgets(buf,100,p);
            fgets(buf,100,p);
            fat[i].current_location = -1;
            continue;
        }
        fat[i].filename = buf;
        fgets(buf, 100,p);
        fat[i].file_length = atoi(buf);
        fgets(buf, 100, p);
        for(int j=0;j<10;j++){
            fat[i].blockPtrs[j] = buf[j];
        }
        fgets(buf, 100, p);
        fat[i].current_location = atoi(buf);
    };

    block_buffer = malloc(partit.block_size);

    // printf("%d\n", partit.block_size);
    // printf("%d\n", partit.total_blocks);
    
    return 1;
}

int openfile(char *name) {
    // search in fat[]
    for(int i=0;i<20;i++){
        if(fat[i].filename == NULL) continue; // no filename 
        if(strcmp(fat[i].filename, name)==0) {
            for(int j=0;j<5;j++) {
                if (fp[j] == NULL) {
                    // FILE *p = fopen("./PARTITION/")
                    files[j].fatIndex = i;
                    return i;
                }
            }
            // no available fp[]
            return -1;
        }
    }
    int newEntry = 0;
    for(int i=0;i<20;i++){
        if(fat[i].filename == NULL) {
            printf("DEBUG: Free FAT at %d\n", i);
            return i;
        }
    }
    // no space in fat
    return -1;
}

int readBlock(int file) {
    if (file == -1) return -1; // error

    int fatIndex = file;
    int pointer = -1;
    for(int i=0;i<5;i++) {
        if(files[i].fatIndex == fatIndex) {
            pointer = i;
        }
    }
    if (pointer == -1) return -1;
    int offset = fat[fatIndex].current_location; // TODO: ??

}