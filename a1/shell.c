#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int parseInput();
int interpreter();

// ==== SHELL MEMORY ====
typedef struct SHELLMEMORY
{
    char *variable;
    char *value;
    struct SHELLMEMORY *next;
} shellMemory;

shellMemory *head = NULL;
shellMemory *tail = NULL;

shellMemory *containsVariable(char *words[])
{
    printf("Searching for variable %s\n", words[1]);

    shellMemory *current = head;
    while (current != NULL)
    {
        if (strcmp(current->variable, words[1]) == 0)
        {
            printf("Found variable %s\n", current->variable);
            printf("Value is %s\n", current->value);
            return current;
        }
        current = current->next;
    }
    printf("Variable %s never existed\n", words[1]);
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
        printf("Value inserted: %s\n", head->value);
        printf("Variable name: %s\n", head->variable);
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

int set(char *words[])
{ // assumes first word is cmd
    if (!containsVariable(words))
    {
        createVariable(words);
    } else {
        containsVariable(words)->value = words[2];
    }
    showShellMemory();
    return 0;
}

int print(char *words[])
{
    printf("looking for %s\n", words[1]);
    shellMemory *target = containsVariable(words);
    if (target) {
       printf("%s\n", target->value); 
    }
    else {
        return 2;
        printf("Variable does not exist\n");
    }
    return 0;
}

int interpreter(char *words[])
{ // assumes words[0] is cmd
    int errCode = 0;

    // if(*(words[0]) == '.' && *(words[0]+1) == '\\') {
    //     errCode = script(words);
    // }

    char *cmd = strdup(words[0]);

    if (strcmp(cmd, "run") == 0)
    {
        printf("Implement script\n");
        // errCode = script(words);
    }
    else if (strcmp(cmd, "quit") == 0)
    {
        printf("Bye!\n");
    }
    else if (strcmp(cmd, "help") == 0)
    {
        char *availableCommands = "COMMAND \t DESCRIPTION\n\n"
                                  "help \t\t Displays all commands\n"
                                  "quit \t\t Exits / terminates the shell with \"Bye!\"\n"
                                  "set VAR STRING \t Assigns a value to shell memory\n"
                                  "print VAR \t Prints the STRING assigned to VAR\n"
                                  "run SCRIPT.TXT \t Executes the file SCRIPT.TXT\n";
        printf("%s", availableCommands);
    }
    else if (strcmp(cmd, "set") == 0)
    {
        errCode = set(words);
    }
    else if (strcmp(cmd, "print") == 0)
    {
        printf("Entered printing mode\n");
        errCode = print(words);
    }
    else
    {
        printf("Unknown command\n");
        errCode = 1;
    }

    return errCode;
}

int parseInput(char ui[])
{
    char *words[1000];
    char tmp[200];

    int a, b;
    int w = 0;

    for (a = 0; ui[a] == ' ' && a < 1000; a++)
        ;
    while (ui[a] != '\0' && a < 1000)
    {
        for (b = 0; ui[a] != '\0' && ui[a] != ' ' && a < 1000; a++, b++)
        {
            tmp[b] = ui[a];
        }
        tmp[b] = '\0';
        words[w] = strdup(tmp);
        a++;
        w++;
    }
    printf("\n");
    return interpreter(words);
}

int main()
{
    printf("Welcome to the Eric Vuong shell!\n");
    printf("Version 1.0 Created January 2019\n");

    char prompt[100] = {'$', '\0'};
    char userInput[1000];
    int errorCode = 0;

    while (1)
    {
        printf("%s", prompt);
        fgets(userInput, 999, stdin);

        // rm endline from fgets
        if (userInput[strlen(userInput) - 1] == '\n')
        {
            userInput[strlen(userInput) - 1] = '\0';
        }

        parseInput(userInput);
    }

    return 0;
}
