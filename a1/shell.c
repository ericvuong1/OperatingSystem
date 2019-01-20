#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

int parseInput(char ui[])
{
    // rm endline from fgets
    if (ui[strlen(ui) - 1] == '\n')
    {
        ui[strlen(ui) - 1] = '\0';
    }

    char *words[1000];
    char tmp[200];

    int a, b;
    int w = 0;

    for (a = 0; ui[a] == ' ' && a < 1000; a++)
        ;
    while (ui[a] != '\0' && a < 1000)
    {
        while (ui[a] == ' ' && a < 1000)
            a++;

        for (b = 0; ui[a] != '\0' && ui[a] != ' ' && a < 1000; a++, b++)
        {
            tmp[b] = ui[a];
        }
        tmp[b] = '\0';
        words[w] = strdup(tmp);
        a++;
        w++;
    }
    return interpreter(words, w);
}

int main()
{
    printf("Welcome to the Eric Vuong shell!\n");
    printf("Version 1.0 Created January 2019\n");

    char prompt[100] = {'$', '\0'};
    char userInput[1000];
    int errorCode = 0;

    while (errorCode == 0)
    {
        printf("%s", prompt);
        fgets(userInput, 999, stdin);

        errorCode = parseInput(userInput);
    }

    return 0;
}
