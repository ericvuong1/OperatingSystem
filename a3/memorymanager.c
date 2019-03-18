#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "helper.h"
#include "pcb.h"
#include "ram.h"

int launcher(FILE *p, char *filename);
int countTotalPages(FILE *f);
FILE *findPage(int pageNumber, FILE *f);
int findFrame(FILE *page);
int findVictim(PCB *p);
int updateFrame(int frameNumber, int victimFrame, FILE*page);
int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame);

// Helper function to concat strings
char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
// Helper function to ceil any positive numbers
int ceiling(float num) {
    int inum = (int) num;
    if (num == (float)inum) {
        return inum;
    }
    return inum + 1;
}

int countTotalPages(FILE *f)
{
    char ch;
    int lines = 0;

    while (!feof(f))
    {
        ch = fgetc(f);
        if (ch == '\n')
            lines++;
    }
    if (lines == 0)
    {
        return 0;
    }

    fclose(f);
    return ceiling((float)lines / 4);
}


FILE *findPage(int pageNumber, FILE *f)
{
    FILE *fp2 = fdopen(dup(fileno(f)), "r");
    int counter = 0;

    char buf[999];
    while (counter < 4 * pageNumber - 1)
    {
        fgets(buf, sizeof(buf), fp2);
        counter++;
    }

    return fp2;
}

int findFrame(FILE *page)
{
    // search through ram[] for a frame not equal to NULL
    // if exists return INDEX else -1
    // Use the FIFO technique to search ram[] for a frame (not equal to NULL).
    // If one exists then return its index number, otherwise return -1.
    for (int i = 0; i < 10; i++)
    {
        if (page == ram[i])
        {
            return i;
        }
    }
    return -1;
}

int findVictim(PCB *p)
{
    // if frame number doesn't belong to pages of PCB
    // return that frame
    int randomNumber = rand() % 10;
    FILE *victim;
    int belong = 0;
    do {
        belong = 0;
        for(int i = 0; i < 10 && !belong; i++) {
            if(p->pageTable[i] == randomNumber) belong = 1;
        }
        if (belong) randomNumber = (randomNumber + 1) % 10;
    } while(belong);
    return randomNumber;
}

int updateFrame(int frameNumber, int victimFrame, FILE *page) {
    if (frameNumber == -1) ram[victimFrame] = page;
    else ram[frameNumber] = page;
    return 1;
}

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame) {
    if (frameNumber == -1) p->pageTable[pageNumber ] = victimFrame;
    else p->pageTable[pageNumber] = frameNumber;
    return 1;
}


int launcher(FILE *p, char *filename)
{
    int pos;
    char ch;

    char *path = concat("./BackingStore/", filename);
    // debug("concat worked");

    // 1. Copy the entire file into the backing store.
    FILE *file = fopen(path, "w");
    fseek(p, 0L, SEEK_END);
    pos = ftell(p);
    fseek(p, 0L, SEEK_SET);
    while (pos--)
    {
        // debug("Wrote stuff");
        // printf("%c\n", ch);
        ch = fgetc(p);
        fputc(ch, file);
    }

    // 2. Close the file pointer pointing to the original file.
    fclose(p);
    fclose(file);

    // 3. Open the file in the backing store.
    FILE *f = fopen(path, "r");

    int totalPages = countTotalPages(f);
    printf("%d pages\n", totalPages);

    FILE *ff = fopen(path, "r");
    FILE *pt = findPage(2, ff);

    // char a[100];
    // fgets(a, sizeof(a), pt);
    // printf("%s\n", a);

    return 1;
}