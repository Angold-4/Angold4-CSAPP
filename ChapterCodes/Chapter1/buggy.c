#include <stdio.h>


/* WARNING: This is buggy code */
float sum_elements(float a[], unsigned length){
    int i;
    float result = 0;

    for (i=0; i <= length-1; i ++)
        result += a[i];
    return result;
}

int main(){
    float a[4] = {1,2,3,4};
    printf("%f\n", sum_elements(a, 0));
    return 0;
}

/* 
 
➜  Chapter1 gcc buggy.c -o buggy
➜  Chapter1 ./buggy
[1]    97579 segmentation fault  ./buggy

*/

/*
 
The Reason is that in line 4 We assign the length as an unsigned char
And When We put the length = 0. in line 15. It will cause segmentation fault:

In line 8, we make a caculation: i <= length-1. And this caculation cause a big problem when length == 0:
It is seems like: i <= 0 - 1, and length(0) is a unsigned char.
When performing an operation, if one operand is signed and the other is unsigned, the C language will implicitly cast the signed parameter to an unsigned number.
So in this case. It is an unsigned operation. And 0 - 1 = -1. -1 will convert into unsigned char(UMax). So this Judgement will always true. And it will access the illegal area of a. 

*/
