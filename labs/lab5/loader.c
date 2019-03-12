#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 80;

int main()
{
    FILE *p;
    int counter = 0;
    while (counter < 5)
    {
        p = fopen("backingstore.txt", "r+wt");
        int n;
        scanf("%d", &n);
        fseek(p, n * 10, SEEK_SET);
        char buff[100];

        fread(buff, sizeof(char), 10, p);
        printf("%s\n", buff);

        counter++;
    }
}