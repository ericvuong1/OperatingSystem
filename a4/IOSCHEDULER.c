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

int requestHead = -1;
int requestTail = -1;

void enqueue(struct REQUEST_QUEUE request) {
    if(((requestTail + 1) % 10) == requestHead ) {printf("Queue is full!\n"); return;}
    if(requestTail == -1) requestHead ++;
    requestTail = (requestTail + 1) % 10;
    requests[requestTail ] = request;
}

struct REQUEST_QUEUE dequeue() {
    // if(requestHead == -1) return NULL;
    if(requestHead == requestTail ) {
        requestHead = -1;
        requestTail = -1;
        struct REQUEST_QUEUE q = {
            .data = "",
            .ptr = NULL,
            .cmd = 0
        };
        return q;
    }
    struct REQUEST_QUEUE request = requests[requestHead ];
    requestHead = (requestHead + 1) % 10;
    return request;
}

int fileToUse = -1;

char *IOscheduler(char *data, struct PCB *ptr, int cmd) {
    struct REQUEST_QUEUE q = {
        .data = data, .ptr = ptr, .cmd = cmd
    };

    enqueue(q);
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