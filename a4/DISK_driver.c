#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int mount(char *name); // load FAT & create buffer_block 1 success 0 fail 
int openfile(char *name); // find filename or creates file if it does not exist, return file's FAT index
int readBlock(int file); // using file FAT index number, load buffer with data from blockID
char *returnBlock(); // return block as string from buffer_block
int writeBlock(int file, char *data); // sensitive to block size, write data to disk at blockID
void writeToDisk();
void loadFromDisk(char *name);


void writeToDiskFAT(int fatIndex);
char *concatStrings(const char *s1, const char *s2);

// https://stackoverflow.com/questions/190229/where-is-the-itoa-function-in-linux/190235
char *my_itoa_buf(char *buf, size_t len, int num)
{
  static char loc_buf[sizeof(int) * 8]; /* not thread safe */

  if (!buf)
  {
    buf = loc_buf;
    len = sizeof(loc_buf);
  }

  if (snprintf(buf, len, "%d", num) == -1)
    return ""; /* or whatever */

  return buf;
}

char *my_itoa(int num)
{ return my_itoa_buf(NULL, 0, num); }

struct PARTITION {
    int total_blocks;
    int block_size;
} partit;

void writeLine(char *s, FILE *p) {
    fputs(s, p);
    while(fgetc(p) != '\n') { // new line
        fseek(p, -1, SEEK_CUR);
        fputc('\0', p);
    }
}

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

char *partitionPath;
char *dataArea;

// Helper function to concat strings
char *concatStrings(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void writeToDisk() {
    FILE *p = fopen(partitionPath, "w");
    fprintf(p, "%d\n", partit.block_size);
    fprintf(p, "%d\n", partit.total_blocks);
    for(int i=0;i<20;i++){

        if(fat[i].filename == NULL) fprintf(p,"NULL\n");
        else fprintf(p, "%s", fat[i].filename);

        fprintf(p, "%d\n", fat[i].file_length);
        for(int j=0;j<10;j++){
            fprintf(p, "%d\n", fat[i].blockPtrs[j]);
        }
        fprintf(p, "%d\n", fat[i].current_location);
    }
    fprintf(p, "%s\n", dataArea);
    fclose(p);
}

void loadFromDisk(char *name) {
    FILE *p = fopen(concatStrings("./PARTITION/", name), "r");
    char buf[100];

    fgets(buf, 100, p);
    partit.block_size = atoi(buf);

    fgets(buf, 100, p);
    partit.total_blocks = atoi(buf);

    for(int i=0;i<20;i++){
        fgets(buf, 100,p); // filename
        if(strcmp("NULL\n", buf)==0){ // skip if null
            printf("DEBUG: empty FAT\n");
            fgets(buf,100,p); //file_length
            for(int j=0;j<10;j++) {
                fgets(buf,100,p); //blockPtrs
                fat[i].blockPtrs[j] = atoi(buf);
                // printf("DEBUG: blockPtr[%d]: %d\n", j, fat[i].blockPtrs[j]);
            }
            fgets(buf,100,p);
            fat[i].current_location = atoi(buf);
            continue;
        }

        printf("DEBUG: found a filename: %s at %d\n", buf, i);
        fat[i].filename = malloc(strlen(buf) + 1);
        strcpy(fat[i].filename, buf);

        fgets(buf,100,p);
        fat[i].file_length = atoi(buf);
        for(int j=0;j<10;j++){
            fgets(buf,100,p);
            fat[i].blockPtrs[j] = atoi(buf);
        }
        fgets(buf,100,p);
        fat[i].current_location = atoi(buf);
    };
}

void initIO() {
    partit.block_size = 0;
    partit.total_blocks = 0;

    block_buffer = NULL;
    for(int i=0;i<5;i++) fp[i]=NULL;
    for(int i=0;i<20;i++) {
        fat[i].current_location = -1;
        fat[i].file_length = -1;
        for(int j=0;j<10;j++){
            fat[i].blockPtrs[j] = -1;
        }
    }
}

int partition(char *name, int blocksize, int totalblocks) {
    printf("DEBUG: Partitioning\n");
    system("mkdir PARTITION");

    // set global partition path 
    char *pathName = concatStrings("./PARTITION/", name);
    partitionPath = pathName;



    FILE * p = fopen(pathName, "w");
    if (p == NULL) return 0;

    // PARTITION
    // append partition data area 
    printf("Block Size: %d\n", blocksize);
    printf("Total Blocks: %d\n", totalblocks);
    fprintf(p, "%d\n", blocksize);
    fprintf(p, "%d\n", totalblocks);

    // EMPTY FAT
    for(int i=0;i<20;i++){
        fprintf(p, "NULL\n"); // filename
        fprintf(p, "-1\n"); // file_length
        for(int j=0;j<10;j++) fprintf(p, "-1\n"); // blockPtr
        fprintf(p, "-1\n"); // current_location
    }
    // DATA AREA
    dataArea = "";
    int totalFreeBlockList = totalblocks * blocksize;
    for(int i=0;i<totalFreeBlockList;i++) dataArea = concatStrings(dataArea, "0");
    fprintf(p, "%s\n", dataArea);
    fclose(p);


    // fclose(p);
    return 1; //TODO: return 0??? when fail?
}
// TODO: not done
int mount(char *name) {

    loadFromDisk(name);
    block_buffer = malloc(partit.block_size);

    printf("MOUNT %d\n", partit.block_size);
    printf("MOUNT %d\n", partit.total_blocks);
    
    return 1;
}

// helper function that returns where data area is in the partition file
FILE *getPtr(int ptr) {
    char buf[100];
    FILE *p = fopen(partitionPath, "r");
    for(int i=0;i<(2+20*13);i++) {
        fgets(buf, 100, p);
        printf("%s\n", buf);
    }
    for(int i=0;i<9*partit.block_size;i++) fgetc(p);
    return p;
} 

int openfile(char *name) {
    // search in fat[]
    for(int i=0;i<20;i++){
        if(fat[i].filename == NULL) continue; // no filename 
        if(strcmp(fat[i].filename, name)==0) { //file found
            for(int j=0;j<5;j++) {
                if (fp[j] == NULL) {
                    FILE *p = getPtr(fat[i].blockPtrs[0]); // get FIRST BLOCK
                    fp[j] = p;
                    return i;
                }
            }
            // no available fp[]
            return -1;
        }
    }

    // new entry
    for(int i=0;i<20;i++){
        if(fat[i].filename == NULL) {
            fat[i].filename = name;
            // for(int j=0;j<10;j++) {
            //     fat[i].blockPtrs[j] = 3;
            // }
            // fat[i].file_length = 5;
            // fat[i].current_location = 8;
            writeToDiskFAT(i);
            printf("DEBUG: BLOCK %d is free\n", i);
            return i;
        }
    }
    return -1;
}

void writeToDiskFAT(int fatIndex) {
    char buff[100];
    FILE *p = fopen(partitionPath, "r+");
    fgets(buff, 100, p);
    fgets(buff, 100, p);
    for(int i=0;i<fatIndex*13;i++) {
        fgets(buff,100,p);
    }
    // filename
    fseek(p, 0, SEEK_CUR);
    writeLine(fat[fatIndex].filename, p);

    // file_length
    fseek(p,0,SEEK_CUR);
    writeLine(my_itoa(fat[fatIndex].file_length), p);

    // blockPtr
    for(int i=0;i<10;i++){
        fseek(p,0,SEEK_CUR);
        writeLine(my_itoa(fat[fatIndex].blockPtrs[i]), p);
    }

    // current_location
    fseek(p,0,SEEK_CUR);
    writeLine(my_itoa(fat[fatIndex].current_location), p);

    fclose(p);
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
    // fat[fatIndex].current_location; // TODO: ??
    FILE *p = fp[pointer];
    return 1;
}

int writeBlock(int file, char *data) {
    return 1;
}