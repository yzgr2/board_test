#include <stdio.h>
#include <math.h>
// #include "dfc.h"
 
#define pi 3.1415926
 
typedef struct { 
float re;// really 
float im;// imaginary 
}complex,*pcomplex; 

complex complexadd(complex a, complex b){ //复数加
    complex rt;
    rt.re = a.re + b.re;
    rt.im = a.im + b.im;
    return rt;
}
 
complex complexMult(complex a, complex b){ //复数乘
    complex rt;
    rt.re = a.re*b.re-a.im*b.im;
    rt.im = a.im*b.re+a.re*b.im;
    return rt;
}
//离散傅里叶变换
void dft(complex X[], complex x[], int N){ //X[]标识变换后频域，x[]为时域采样信号，下同
    complex temp;
    int k, n;
    for (int k = 0; k < N; k++)
    {
        X[k].re = 0;
        X[k].im = 0;
        for (int n = 0; n < N; n++)
        {
            temp.re = (float)cos(2*pi*k*n/N);
            temp.im = -(float)sin(2*pi*k*n/N);
            X[k] = complexadd(X[k], complexMult(x[n],temp));
 
        }
         
    }
}
//离散傅里叶逆变换
void idft(complex X[], complex x[], int N){
    complex temp;
    int k, n;
    for (int k = 0; k < N; k++)
    {
        x[k].re = 0;
        x[k].im = 0;
        for (int n = 0; n < N; n++)
        {
            temp.re = (float)cos(2*pi*k*n/N);
            temp.im = (float)sin(2*pi*k*n/N);
            x[k] = complexadd(x[k], complexMult(X[n],temp));
 
        }
        x[k].re /= N;
        x[k].im /= N;
    }
}

complex test_data[] = {
{ 31, 1 },
{ 27, 2 },
{ 21, -2 },
{ 24, 1 },
{ 7, 2 },
{ 13, -5 },
{ 12, -7 },
{ 5, -6 },
{ 9, -14 },
{ 3, -25 },
{ 7, -24 },
{ 1, -28 },
{ 9, -22 },
{ 10, -27 },
{ 9, -25 },
{ 10, -26 },
{ 8, -30 },
{ 10, -22 },
{ 4, -17 },
{ 6, -16 },
{ 3, -14 },
{ -4, -12 },
{ -11, -10 },
{ -13, -11 },
{ -12, -5 },
{ 0, 0 },
{ 0, 0 },
{ 0, 0 },
{ 0, 0 },
{ 0, 0 },
{ 0, 0 },
{ 0, 0 },
{ 0, 0 },
{ 0, 0 },
{ 0, 0 },
{ 0, 0 },
{ -8, 1 },
{ 0, 4 },
{ -6, 3 },
{ 6, 4 },
{ 8, 7 },
{ 10, 12 },
{ 19, 5 },
{ 27, 17 },
{ 21, 17 },
{ 17, 16 },
{ 15, 10 },
{ 21, 20 },
{ 12, 18 },
{ 5, 22 },
{ 11, 18 },
{ 9, 13 },
{ 15, 8 },
{ 8, 2 },
{ 13, 1 },
{ 9, -4 },
{ 13, -3 },
{ 13, 1 },
{ 20, -11 },
{ 22, -14 },
{ 26, -9 },
{ 32, -4 }
};

void print_array_complex(complex *ori, int N)
{
    for( int i=0; i<N; i++) {
        printf(" %f + (%fj), ", ori[i].re, ori[i].im);
    }
}

#include<stdlib.h>

int main()
{
    complex *ori = malloc(128 * sizeof(complex));

    idft(test_data, ori, sizeof(test_data)/sizeof(complex));

    print_array_complex(ori, sizeof(test_data)/sizeof(complex));

    return 0;
}


//gcc -o ifft ifft.c -lm