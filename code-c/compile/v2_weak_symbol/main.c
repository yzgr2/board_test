#include<stdio.h>

extern int var_g;
void set_varg(int g);
extern int func1(int v);
extern int func2(int v);

int main()
{    
    printf("func1 result:%d\n", func1(1));
    printf("func2 result:%d\n", func2(1));
    printf("main.c: addr of var_g:%p\n", &var_g);	
   //printf("version=%d\n", func(0));
    printf("sizeof(var_g)=%lu ",sizeof(var_g));
}
