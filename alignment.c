#include <stdio.h>

struct an_struct{
    int i;
    int j;
    int  k;
    char l;
    char c;
    char *m;
    long n;
};

int main(){
    struct an_struct an_guy;
    an_guy.m = "hello, world?";
    printf("%p\n", &an_guy.i);
    printf("%p\n", &an_guy.j);
    printf("%p\n", &an_guy.k);
    printf("%p\n", &an_guy.l);
    printf("%p\n", &an_guy.c);
    printf("%p\n", &an_guy.m);
    printf("%p\n", &an_guy.n);
}
