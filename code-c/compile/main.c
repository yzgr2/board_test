#include<stdio.h>

extern int func(int);

int main()
{
  printf("version=%d\n", func(0));
}
