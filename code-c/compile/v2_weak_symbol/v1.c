#include<stdio.h>

int a;

//not initialized, it is weak symbol.
int var_g;


int func1(int v)
{
   printf("v1.c : sizeof(var_g)=%lu  addr of var_g:%p   &a:%p\n", sizeof(var_g), &var_g, &a);
   return v + var_g ;
}

void set_varg(int g)
{ 
   var_g = g ;
}
