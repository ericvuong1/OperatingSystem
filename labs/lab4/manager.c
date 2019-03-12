#include <stdlib.h>
#include <stdio.h>
#include "driver.h"

FILE *single_user_ptr[5];
char single_user_buffer;
char *single_user_file_name;

int api_fopen(char *filename);
int api_fputs(char output);
int api_fclose();

int manager_consumer_fopen()
{
    single_user_ptr[0] = driver_open(single_user_file_name);
    return 0;
}

int manager_consumer_fclose()
{
    return driver_fclose(single_user_ptr);
}

int manager_consumer_fputs()
{
    printf("%c\n", single_user_buffer);
    printf("HO\n");
    driver_put(single_user_buffer, single_user_ptr[0]);
    manager_consumer_fclose();
    return 0;
}


int api_fopen(char *filename)
{
    single_user_file_name = filename;
    manager_consumer_fopen();
    return 0;
}

int api_fputs(char output)
{
    single_user_buffer = output;
    manager_consumer_fputs();
    return 0;
}

int api_fclose()
{
    manager_consumer_fclose();
    return 0;
}
