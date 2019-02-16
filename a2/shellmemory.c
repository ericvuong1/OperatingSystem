#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shellmemory.h"

shellMemory *containsVariable(char *words[]);
void createVariable(char *words[]);
void showShellMemory();

// ==== SHELL MEMORY ====

shellMemory *head = NULL;
shellMemory *tail = NULL;

shellMemory *containsVariable(char *words[])
{
    shellMemory *current = head;
    while (current != NULL)
    {
        if (strcmp(current->variable, words[1]) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return 0;
}

void createVariable(char *words[])
{
    if (head == NULL)
    {
        head = malloc(sizeof(shellMemory));
        head->variable = words[1];
        head->value = words[2];
        head->next = NULL;
        tail = head;
    }
    else
    {
        tail->next = malloc(sizeof(shellMemory));
        tail->next->variable = words[1];
        tail->next->value = words[2];
        tail->next->next = NULL;
        tail = tail->next;
    }
}

void showShellMemory()
{
    printf("Current Shell Memory:\n");

    shellMemory *current = head;
    while (current != NULL)
    {
        printf("%s (%s) -> ", current->variable, current->value);
        current = current->next;
    }
    printf("null\n");
}
