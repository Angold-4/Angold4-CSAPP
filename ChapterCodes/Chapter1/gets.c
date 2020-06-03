# include<stdio.h>

char *gets(*s){
    int c;
    char *dest = s;
    while ((c = getchar()) != '\n' && c != EOF)
        *dest++ = c;
    if (c == EOF && dest == s)
        return NULL;
    *dest++= '\0';
    return s;
}

int main(){
    char *s;
    gets(s);
    return 0;
}
