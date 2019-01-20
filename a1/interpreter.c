#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "shellmemory.h"

// public
int interpreter(char *words[], int count);

// private
int run(char *words[]);
int quit();
int help();
int set(char *words[], int count);
int print(char *words[]);
int unknownCommand();

int set(char *words[], int count) // assumes first word is cmd
{
    if (count < 3 || *words[1] == '\0' || *words[2] == '\0')
    {
        printf("Invalid set command\n");
        return 0;
    }
    if (!containsVariable(words))
    {
        createVariable(words);
    }
    else
    {
        containsVariable(words)->value = words[2];
    }
    printf("Variable %s has been set to %s\n", words[1], words[2]);
    // showShellMemory();
    return 0;
}

int quit()
{
    printf("Bye!\n");
    return -1;
}

int unknownCommand()
{
    printf("Unknown command\n");
    return 0;
}

int help()
{
    char *availableCommands = "COMMAND \t DESCRIPTION\n\n"
                              "help \t\t Displays all commands\n"
                              "quit \t\t Exits / terminates the shell with \"Bye!\"\n"
                              "set VAR STRING \t Assigns a value to shell memory\n"
                              "print VAR \t Prints the STRING assigned to VAR\n"
                              "run SCRIPT.TXT \t Executes the file SCRIPT.TXT\n";
    printf("%s", availableCommands);
    return 0;
}

int print(char *words[])
{
    shellMemory *target = containsVariable(words);
    if (target)
    {
        printf("%s\n", target->value);
    }
    else
    {
        printf("Variable does not exist\n");
        return 0;
    }
    return 0;
}

int interpreter(char *words[], int count)
{ // assumes words[0] is cmd
    int errCode = 0;

    char *cmd = strdup(words[0]);

    if (strcmp(cmd, "run") == 0 && count > 1)
        errCode = run(words);
    else if (strcmp(cmd, "quit") == 0)
        errCode = quit();
    else if (strcmp(cmd, "help") == 0)
        errCode = help();
    else if (strcmp(cmd, "set") == 0)
        errCode = set(words, count);
    else if (strcmp(cmd, "print") == 0 && count > 1)
        errCode = print(words);
    else
        errCode = unknownCommand();

    return errCode;
}

int run(char *words[])
{
    int errCode = 0;
    char line[1000];
    FILE *p = fopen(words[1], "rt");

    if (p == NULL)
    {
        printf("Script not found\n");
        return 0;
    }

    while (p != NULL && fgets(line, 999, p))
    {
        errCode = parseInput(line);
        if (errCode != 0)
        {
            fclose(p);
            return errCode;
        }
    }

    fclose(p);
    return errCode;
}