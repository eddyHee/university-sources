#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int number;
    char* name;
} Data;

/*
 * This function should create n Data structs, print out i for each one
 */
int main()
{
    char a[] = "Hello";
    Data* data [10];
    char *test;
    for (int i = 0; i < sizeof(data)/sizeof(data[0]); i++)
    {
        data[i]->number = i;
        // change this to sprintf, so line 24 can work
        printf("%d test\n", data[i]->number);
    }
    printf("%s\n", data[8]->name);

}
