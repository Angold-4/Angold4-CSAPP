#include <stdio.h>


char *gets(char * s){
    /*int c;*/
    char *dest = s;
    printf("%c \n", *dest);
    printf("%p \n", dest);
    printf("%c \n", *s);
    return 0;
}


int main(){
    char b = '1';
    gets(&b);
    printf("%p \n", &b);
    return 0;
}

/*1*/
/*0x7ffeef18282b*/
/*1*/
/*0x7ffeef18282b*/
