#include<stdio.h>
#include<math.h>

int bright2p(unsigned short brightness)
{
  double p;
  p = brightness;
  p = p / 65535;  //>5 to ceil, <5 to floor
  p += 0.005;
  p *= 100;

  //printf("bright2p: bright:%u p:%lf ori_p:%lf percent:%d\n", brightness, p, p-0.5, (unsigned short)p );
  return p; 
}

unsigned short p2bright(int percent)
{
  double b;
  b = (double)(percent) * 65535 / 100 + 0.5;

  //printf("p2bright: percent:%d bright:%lf brightness:%u\n", percent, b, (unsigned short)b );

  return b;
}



#if 1
int main()
{
  int i;
  unsigned short bval;
  unsigned short rev;

  for(i=0; i<=100; i++)
  {
	bval = p2bright(i);
	rev = bright2p(bval);

	if( rev != i )
	{
	    printf("!!!ERR: percent:%d  brightess:%u  percent:%d\n\n", i, bval, rev );
	}
	else
	{	
        	printf("percent=%d brightness=%u\n\n", i, bval);	
	}
  }
}
#endif
