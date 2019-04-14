#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int mount(char *name);                // load FAT & create buffer_block 1 success 0 fail
int openfile(char *name);             // find filename or creates file if it does not exist, return file's FAT index
int readBlock(int file);              // using file FAT index number, load buffer with data from blockID
char *returnBlock();                  // return block as string from buffer_block
int writeBlock(int file, char *data); // sensitive to block size, write data to disk at blockID
FILE *getPtr(int ptr);

void writeToDisk();
void loadFromDisk(char *name);
char *appendCharToString(char *str, char c);
int findSpace();
void writeToDiskFAT(int fatIndex);
char *concatStrings(const char *s1, const char *s2);
int myceil(float num);
void clearBlock(int blockNumber);

char EMPTY_DATA[100] = "                                                                                        \n";

// variables

struct PARTITION
{
    int total_blocks;
    int block_size;
} partit;

struct FAT
{
    char *filename;
    int file_length;
    int blockPtrs[10];
    int current_location;
} fat[20];

struct FILE_DS
{
    int fatIndex;
} files[5];

char *block_buffer;
FILE *fp[5];

char *partitionPath;
char *dataArea;

char *returnBlock()
{
    return block_buffer;
}

int myceil(float num)
{
    int inum = (int)num;
    if (num == (float)inum)
    {
        return inum;
    }
    return inum + 1;
}

int findSpace()
{
    FILE *p = getPtr(0);
    for (int i = 0; i < partit.total_blocks; i++)
    {
        if (fgetc(p) == '0')
        {
            return i;
        }
        for (int j = 0; j < partit.block_size - 1; j++)
            fgetc(p);
    }
    fclose(p);
    return -1;
}

char *appendCharToString(char *str, char c)
{

    size_t len = strlen(str);
    char *str2 = malloc(len + 1 + 1); /* one for extra char, one for trailing zero */
    strcpy(str2, str);
    str2[len] = c;
    str2[len + 1] = '\0';
    return str2;
}

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
{
    return my_itoa_buf(NULL, 0, num);
}

void writeLine(char *s, FILE *p)
{
    s = concatStrings(s, "\0");
    fputs(s, p);
    while (fgetc(p) != '\n')
    { // new line
        fseek(p, -1, SEEK_CUR);
        fputc('\0', p);
    }
}

// Helper function to concat strings
char *concatStrings(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void writeToDisk()
{
    FILE *p = fopen(partitionPath, "w");
    fprintf(p, "%d\n", partit.block_size);
    fprintf(p, "%d\n", partit.total_blocks);
    for (int i = 0; i < 20; i++)
    {

        if (fat[i].filename == NULL)
            fprintf(p, "%s", EMPTY_DATA);
        else
            fprintf(p, "%s\n", fat[i].filename);

        fprintf(p, "%d\n", fat[i].file_length);
        for (int j = 0; j < 10; j++)
        {
            fprintf(p, "%d\n", fat[i].blockPtrs[j]);
        }
        fprintf(p, "%d\n", fat[i].current_location);
    }
    fprintf(p, "%s\n", dataArea);
    fclose(p);
}

void loadFromDisk(char *name)
{
    FILE *p = fopen(concatStrings("./PARTITION/", name), "r");
    char buf[100];

    fgets(buf, 100, p);
    partit.block_size = atoi(buf);

    fgets(buf, 100, p);
    partit.total_blocks = atoi(buf);

    for (int i = 0; i < 20; i++)
    {
        fgets(buf, 100, p); // filename
        if (strcmp(EMPTY_DATA, buf) == 0)
        { // skip if null
            // printf("DEBUG: empty FAT\n");
            fgets(buf, 100, p); //file_length
            for (int j = 0; j < 10; j++)
            {
                fgets(buf, 100, p); //blockPtrs
                fat[i].blockPtrs[j] = atoi(buf);
                // printf("DEBUG: blockPtr[%d]: %d\n", j, fat[i].blockPtrs[j]);
            }
            fgets(buf, 100, p);
            fat[i].current_location = -1;
            continue;
        }

        // printf("DEBUG: found a filename: %s at %d\n", buf, i);
        fat[i].filename = malloc(strlen(buf) + 1);
        strcpy(fat[i].filename, buf);

        fgets(buf, 100, p);
        fat[i].file_length = atoi(buf);
        for (int j = 0; j < 10; j++)
        {
            fgets(buf, 100, p);
            fat[i].blockPtrs[j] = atoi(buf);
        }
        fgets(buf, 100, p);
        // fat[i].current_location = atoi(buf);
        fat[i].current_location = -1;
    };
}

void initIO()
{
    partit.block_size = 0;
    partit.total_blocks = 0;

    block_buffer = NULL;
    for (int i = 0; i < 5; i++)
    {
        fp[i] = NULL;
        files[i].fatIndex = -1;
    }
    for (int i = 0; i < 20; i++)
    {
        fat[i].current_location = -1;
        fat[i].file_length = -1;
        for (int j = 0; j < 10; j++)
        {
            fat[i].blockPtrs[j] = -1;
        }
    }
}

/*
 * Check if a file exist using fopen() function
 * return 1 if the file exist otherwise return 0
 */
int cfileexists(const char *filename)
{
    /* try to open file to read */
    FILE *file;
    if ((file = fopen(filename, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int partition(char *name, int blocksize, int totalblocks)
{
    // printf("DEBUG: Partitioning\n");
    system("mkdir PARTITION");

    // set global partition path
    char *pathName = concatStrings("./PARTITION/", name);
    partitionPath = pathName;

    if (cfileexists(pathName))
        return 1; // file exists already, so mount it

    FILE *p = fopen(pathName, "w");
    if (p == NULL)
        return 0;

    // PARTITION
    // append partition data area
    // printf("Block Size: %d\n", blocksize);
    // printf("Total Blocks: %d\n", totalblocks);
    fprintf(p, "%d\n", blocksize);
    fprintf(p, "%d\n", totalblocks);

    // EMPTY FAT
    for (int i = 0; i < 20; i++)
    {
        fprintf(p, "%s", EMPTY_DATA); // filename
        fprintf(p, "-1\n");           // file_length
        for (int j = 0; j < 10; j++)
            fprintf(p, "-1\n"); // blockPtr
        fprintf(p, "-1\n");     // current_location
    }
    // DATA AREA
    dataArea = "";
    int totalFreeBlockList = totalblocks * blocksize;
    for (int i = 0; i < totalFreeBlockList; i++)
        dataArea = concatStrings(dataArea, "0");
    fprintf(p, "%s\n", dataArea);
    fclose(p);

    // fclose(p);
    return 1; //TODO: return 0??? when fail?
}

int mount(char *name)
{
    // printf("DEBUG: mounting %s\n", name);

    loadFromDisk(name);
    block_buffer = malloc(partit.block_size);

    // printf("MOUNT %d\n", partit.block_size);
    // printf("MOUNT %d\n", partit.total_blocks);

    return 1;
}

// helper function that returns where data area is in the partition file
FILE *getPtr(int ptr)
{
    // printf("DEBUG: inside getPtr\n");
    char buf[100];
    FILE *p = fopen(partitionPath, "r+");
    for (int i = 0; i < (2 + 20 * 13); i++)
    {
        fgets(buf, 100, p);
    }
    for (int i = 0; i < ptr * partit.block_size; i++)
        fseek(p, 1, SEEK_CUR);
    return p;
}

int openfile(char *name)
{
    // printf("DEBUG filename[0] %s\n", fat[0].filename);
    // printf("DEBUG: opening file %s\n", name);
    // search in fat[]
    for (int i = 0; i < 20; i++)
    {
        if (fat[i].filename == NULL)
            continue; // no filename
        if (strcmp(fat[i].filename, name) == 0)
        { //file found
            // printf("DEBUG: File %s is same as %s found at fat index %d\n", fat[i].filename, name, i);

            // check if fp[] already open for fatindex i
            for (int j = 0; j < 5; j++)
            {
                if (files[j].fatIndex == i)
                {
                    fp[j] = getPtr(fat[i].blockPtrs[0]); // get FIRST BLOCK
                    // printf("DEBUG: File %d already opened, using fp[%d]\n", i, j);
                    // fat[i].current_location = 0;
                    return i;
                }
            }
            // check if there's an fp[] available
            for (int j = 0; j < 5; j++)
            {
                if (fp[j] == NULL)
                {
                    // printf("DEBUG: found the filename %s, setting fp[%d]\n", name, j);
                    fp[j] = getPtr(fat[i].blockPtrs[0]); // get FIRST BLOCK
                    // fat[i].current_location = 0;
                    return i;
                }
            }
            // no available fp[]
            printf("No available fp[]\n");
            return -1;
        }
    }

    // new entry
    for (int i = 0; i < 20; i++)
    {
        if (fat[i].filename == NULL)
        {
            // printf("DEBUG: new entry for %s\n", name);
            fat[i].filename = name;
            writeToDiskFAT(i);
            // writeToDisk();
            // printf("DEBUG: FAT %d is free\n", i);
            return i;
        }
    }
    return -1;
}

void writeToDiskFAT(int fatIndex)
{
    char buff[100];
    FILE *p = fopen(partitionPath, "r+");
    fgets(buff, 100, p);
    fgets(buff, 100, p);
    for (int i = 0; i < fatIndex * 13; i++)
    {
        fgets(buff, 100, p);
    }
    // filename
    fseek(p, 0, SEEK_CUR);
    writeLine(fat[fatIndex].filename, p);

    // file_length
    fseek(p, 0, SEEK_CUR);
    writeLine(my_itoa(fat[fatIndex].file_length), p);

    // blockPtr
    for (int i = 0; i < 10; i++)
    {
        fseek(p, 0, SEEK_CUR);
        writeLine(my_itoa(fat[fatIndex].blockPtrs[i]), p);
    }

    // current_location
    fseek(p, 0, SEEK_CUR);
    writeLine(my_itoa(fat[fatIndex].current_location), p);

    fclose(p);
}

void clearBlock(int blockNumber)
{
    FILE *p = getPtr(blockNumber);
    fseek(p, 0, SEEK_CUR);
    for (int i = 0; i < partit.block_size; i++)
    {
        fputc('0', p);
    }
    fclose(p);
}

char *readFile(int file)
{
    if (file == -1)
        return NULL; // error

    int pointer = -1;
    for (int i = 0; i < 5; i++)
    {
        // printf("DEBUG checking files[%d] and has fatIndex %d\n", i, files[i].fatIndex);
        if (files[i].fatIndex == file)
        {
            pointer = i;
        }
    }
    // printf("DEBUG: attempt to write %s at file %d with pointer %d\n", data, file, pointer);
    if (pointer == -1)
    {
        // printf("DEBUG: no pointer exists\n");

        // printf("DEBUG: finding an available fp[]\n");
        for (int i = 0; i < 5; i++)
        {
            if (fp[i] == NULL)
            {
                files[i].fatIndex = file;
                pointer = i;
                break;
            }
        }
        // no available fp to write
        if (pointer == -1)
        {
            printf("No fp[] available\n");
            return NULL;
        } // empty file
    }

    FILE *p = fp[pointer];

    int blockNumber = fat[file].blockPtrs[0];
    if (blockNumber == -1)
        return NULL; // empty file

    // int ptr = fat[file].current_location;
    // if (ptr == -1) ptr = 0;

    int ptr = 0; // Always read at beginning of file
    fat[file].current_location = ptr;

    // printf("DEBUG: current location is %d\n", ptr);

    char *str = "";
    for (int i = fat[file].current_location; i < 10; i++)
    {
        // printf("DEBUG: at file %d and blockPtr[%d] has %d\n", file, i, fat[file].blockPtrs[i]);
        // printf("DEBUG: current location after read is %d\n", fat[file].current_location);
        if (readBlock(fat[file].blockPtrs[i]) == -1)
        {
            return str;
        };
        str = concatStrings(str, returnBlock());
        fat[file].current_location++;
    }
    // printf("READ: %s\n", str);
    return str;
}

int readBlock(int file)
{
    if (file == -1)
        return -1;
    // printf("Reading block %d\n", file);
    FILE *p = getPtr(file);
    char *str = "";
    for (int i = 0; i < partit.block_size; i++)
    {
        char a = fgetc(p);
        block_buffer[i] = a;
        // printf("DEBUG: read character %c\n", a);
    }
    // printf("DEBUG: buffer updated to %s\n", block_buffer);

    return 1;
}

int writeBlock(int file, char *data)
{
    if (file == -1)
        return -1; // error

    int pointer = -1;
    for (int i = 0; i < 5; i++)
    {
        // printf("DEBUG checking files[%d] and has fatIndex %d\n", i, files[i].fatIndex);
        if (files[i].fatIndex == file)
        {
            pointer = i;
        }
    }
    // printf("DEBUG: attempt to write %s at file %d with pointer %d\n", data, file, pointer);
    if (pointer == -1)
    {
        // printf("DEBUG: no pointer exists\n");

        // printf("DEBUG: finding an available fp[]\n");
        for (int i = 0; i < 5; i++)
        {
            if (fp[i] == NULL)
            {
                files[i].fatIndex = file;
                pointer = i;
                break;
            }
        }
        // no available fp to write
        if (pointer == -1)
        {
            printf("No fp[] available\n");
            return -1;
        }
    }

    FILE *p = fp[pointer];

    int length = strlen(data);
    int lastBlock = myceil((float)length / partit.block_size);
    int blockCounter = 0;

    int ptr;
    ptr = fat[file].current_location;
    // ptr = 0;

    if (ptr == -1)
        ptr = 0;

    while (blockCounter < lastBlock)
    {
        int blockNumber = fat[file].blockPtrs[ptr];
        if (blockNumber == -1)
            blockNumber = findSpace(); // find available space
        if (blockNumber == -1)
        {
            printf("No more available space to write\n");
            return -1;
        }
        fat[file].blockPtrs[ptr] = blockNumber;
        clearBlock(blockNumber);

        fp[pointer] = getPtr(blockNumber);

        int counter = partit.block_size;
        while (counter > 0)
        {
            fseek(fp[pointer], 0, SEEK_CUR);
            fputc(*data, fp[pointer]);
            length = length - 1;
            if (length == 0)
                break; // done writing
            data = data + 1;
            counter = counter - 1;
        }
        ptr = ptr + 1;
        fat[file].current_location = ptr;
        blockCounter = blockCounter + 1;
        writeToDiskFAT(file);
        // writeToDisk();
        fclose(fp[pointer]);
    }
    int file_length = 0;
    for (int i = 0; i < 10; i++)
    {
        if (fat[file].blockPtrs[i] != -1)
            file_length++;
    }
    fat[file].file_length = file_length;
    writeToDiskFAT(file);
    // writeToDisk();

    return 1;
}
