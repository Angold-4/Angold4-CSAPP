/*This is a very low-quality code*/
#include<string.h>
#include<stdio.h>
#include<stdlib.h>


char *get_line()
{
    char buf[4];
    char *result;
    gets(buf);
    result = malloc(strlen(buf)+1);
    strcpy(result, buf);
    return result;
}


int main(){
    get_line();
    return 0;
}
