#include <stdio.h>
#include <string.h>

int parseInput();
int interpreter();

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
    } else if (strcmp(cmd, "quit") == 0) {
        printf("Quit\n");
    } else if (strcmp(cmd, "help") == 0) {
        printf("Help\n");
    } else if (strcmp(cmd, "set") == 0) {
        printf("SET\n");
    } else if (strcmp(cmd, "print") == 0) {
        printf("PRINT\n");
    }

    return 1;
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
