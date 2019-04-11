#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "pcb.h"

char *IOscheduler(char *data, PCB *ptr, int cmd);

// Circular Array
struct REQUEST_QUEUE {
    char *data;
    PCB *ptr;
    int cmd;
} requests[10];

char *IOscheduler(char *data, PCB *ptr, int cmd) {
    if (cmd == 1) {
        // write
    } else if (cmd == 0) {
        // read
    }
    return "";
}