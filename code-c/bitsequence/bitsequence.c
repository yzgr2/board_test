#include<stdio.h>

struct test_bits
{
    unsigned char d  : 1;
    unsigned char d2 : 1;
    unsigned char d3 : 6;
};

int main()
{
#ifdef BITS_BIG_ENDIAN
  printf("GCC defined BITS_BIG_ENDIAN\n");
#else
  printf("GCC not defined BITS_BIG_ENDIAN.\n");
#endif

  struct test_bits t;
  unsigned char *p = (unsigned char *)&t;
  t.d = 1;
  t.d2 = 1;
  t.d3 = 0;

  printf("*p = %u \n", *p );

}
