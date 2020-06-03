#include <stdio.h>


int linker(unsigned word0, unsigned word1){
    union {
        int d;
        short u[2];
    }temp;

    temp.u[0] = word0;
    temp.u[1] = word1;
    return temp.d;
}


int main(){
    int val = linker(1, 0);
    printf("%d", val);
    return 0;
}
