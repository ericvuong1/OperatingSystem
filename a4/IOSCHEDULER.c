#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "pcb.h"
#include "DISK_driver.h"

char *IOscheduler(char *data, struct PCB *ptr, int cmd);

// Circular Array
struct REQUEST_QUEUE {
    char *data;
    PCB *ptr;
    int cmd;
} requests[10];



int fileToUse = -1;

char *IOscheduler(char *data, struct PCB *ptr, int cmd) {
    if (cmd == 1) {
        // write
        writeBlock(fileToUse, strdup(data));
    } else if (cmd == 0) {
        // read
        char * result = readFile(fileToUse);
        if (result == NULL) return ""; // if it's an empty file
        return result;
    }
    return "";
}