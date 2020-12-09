#include<stdio.h>

extern int func(int);
extern int test(int t);

int main()
{
   test(100);
   printf("version=%d\n", func(0));
}
