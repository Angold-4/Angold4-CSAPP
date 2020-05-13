#include <stdio.h>


void div16(int x){
    int result = 0;
    result = (x + 15) >> 4;
    printf("%d\n", result);
}

void Div16(int x){
    int result = 0;
    /* Compute PN to be either 0 (x >= 0) or 15 (x < 0) */
    int PN = (x >> 31) & 0xF;
    result = (x + PN) >> 4;
    printf("%d\n", result);
}

int main(){
    div16(123);
    Div16(123);
    return 0;
}

/*
 
➜  Chapter1 (master) ✗ gcc div16.c -o div16
➜  Chapter1 (master) ✗ ./div16
-7

*/

/*
 
Preference: https://www.programiz.com/c-programming/bitwise-operators
12 = 00001100 (In Binary)
25 = 00011001 (In Binary)

Bit Operation of 12 and 25
  00001100
& 00011001
  ________
  00001000  = 8 (In decimal)

*/

