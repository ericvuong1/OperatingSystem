#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

int parse(char ui[]);

int parse(char ui[])
{
    // rm endline from fgets
    if (ui[strlen(ui) - 1] == '\n')
    {
        ui[strlen(ui) - 1] = '\0';
    }

    char *words[1000];
    char tmp[1000];

    int a, b;
    int w = 0;
    // rm spaces
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
        words[w] = strdup(tmp); // extract words
        a++;
        if(*words[w] != '\0') // takes care of extra spaces doesn't count on words
            w++;
    }
    words[w] = NULL; // set last word to null to know end of ui
    return interpreter(words, w); 
}

// int main()
// {
//     printf("Welcome to the Eric Vuong shell!\n");
//     printf("Version 1.0 Created January 2019\n");

//     char prompt[100] = {'$', '\0'};
//     char userInput[1000];
//     int errorCode = 0;
//     while (errorCode == 0 && !feof(stdin))
//     {
//         printf("%s", prompt);
//         if (fgets(userInput, 999, stdin) == NULL)
//             continue;

//         errorCode = parse(userInput);
//     }

//     return 0;
// }
