#include<stdio.h>

int b;

//not initialized, it is weak symbol.
long long var_g = 100 ;

int func2(int v)
{
   printf("v2.c : sizeof(var_g)=%lu  addr of var_g:%p  &b:%p\n", sizeof(var_g), &var_g, &b);
   return v + var_g + 1;
}


